module Module where
{- Useful functions for collecting metadata about modules etc. -}

import Parser (Identifier(..), Instruction(..))
import qualified Data.Map as Map
import Data.List ((\\), nub, intercalate, intersect)

type Fred = ([String], [String], [(String, [Instruction])]) -- modules, imports, definitions
type Fmod = ([(String, [Identifier])], String)

type Prog = (Map.Map String [Instruction])
type Module = ([Identifier], [String], [String], String)
--              module name, implements, declares, requires, blob -- TODO: change to greedily append module name

getDefined :: Fred -> [String]
getDefined (_, _, x) = let (y, _) = unzip x in y

getExported :: Fred -> [String]
getExported fred = nub (getDefined fred)

getRedefined :: Fred -> [String]
getRedefined fred = nub (getDefined fred \\ getExported fred)

unwrapIns :: Instruction -> Identifier
unwrapIns (Call x) = x
unwrapIns (Push x) = x

getUtilized :: Fred -> [Identifier]
getUtilized (_, _, x) = let (_, y) = unzip x in
    nub . concatMap (map unwrapIns) $ y

getExternal :: Fred -> [Identifier]
getExternal fred = filter (\e -> not (e `elem` (getDefined fred))) (getUtilized fred)

makeProg :: Fred -> [Identifier] -> [String] -> Either String Prog
makeProg fred@(_, _, defs) supplied required = case (getExternal fred \\ supplied) of
    diff@(_:_) -> Left ("words " ++ intercalate ", " (map (show . show) diff) ++ " are undefined")
    [] -> case getRedefined fred of
        redef@(_:_) -> Left ("words " ++ intercalate ", " (map show redef) ++ " are internally redefined")
        [] -> case required \\ getDefined fred of
            req@(_:_) -> Left ("words " ++ intercalate ", " (map show req) ++ " need to be defined")
            [] -> case nub (intersect (getExported fred) supplied) of
                redef@(_:_) -> Left ("words " ++ intercalate ", " (map (show . show) redef) ++ " are redefinitions of external functions")
                [] -> Right (Map.fromList defs)

getFields :: Fmod -> [String]
getFields (x, _) = let (y, _) = unzip x in y

getDuplicateFields :: Fmod -> [String]
getDuplicateFields fmod = nub (getFields fmod \\ nub (getFields fmod))

makeModule :: Fmod -> Either String Module
makeModule fmod@(f, blob) = case getDuplicateFields fmod of
    [] -> let fields = Map.fromList f in case do {
        defines <- "defines" `Map.lookup` fields;
        declares <- "declares" `Map.lookup` fields;
        requires <- "requires" `Map.lookup` fields;
        return (defines, declares, requires);
        } of
            Just (defines, declares, requires) -> Right (defines, declares, requires, blob)
            Nothing -> Left "could not parse module file"

includeModule :: Module -> String -> Files -> Either String Files
includeModule mod name files@(mods, progs) = Right (Map.insert name mod mods, progs)

genModuleDefs :: Module -> [Identifier]
genModuleDefs (x, _, _, _) = x

genModuleReqs :: Module -> [String]
genModuleReqs (_, x, _, _) = x

type Files = (Map.Map String Module, Map.Map String Prog)
type Request = ([String], [String])
--      requires modules, programs

bulkLookup :: (Ord k) => [k] -> Map.Map k a -> Either [k] [a]
bulkLookup keys map = case arf keys [] [] map of
    ([], as) -> Right as
    (ks, _) -> Left ks
    where
        arf (k:ks) keys values map = case k `Map.lookup` map of
            Just x -> arf ks keys (x:values) map
            Nothing -> arf ks (k:keys) values map
        arf [] ks vs _ = (ks, vs)

loadWith :: Fred -> Files -> Either String (Either Request Prog)
loadWith fred@(modules, imports, _) (loadedMod, loadedProg) =
    case (bulkLookup modules loadedMod, bulkLookup imports loadedProg) of
        (Left xs, Left ys) -> Right (Left (xs, ys))
        (Left xs, _) -> Right (Left (xs, []))
        (_, Left xs) -> Right (Left ([], xs))
        (Right xs, Right ys) ->
            let supplied = concatMap genModuleDefs xs ++ concatMap Map.keys ys
                required = concatMap genModuleReqs xs in
                    case makeProg fred supplied required of
                        Left err -> Left err
                        Right prog -> Right (Right prog)