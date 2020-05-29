module Cgen where
import Module
import Parser
import qualified Data.Map as Map
import Data.List (nub, unzip4, (\\), intercalate)

getModules :: Files -> [Module]
getModules (mods, _) = Map.elems mods

genHeader :: [Module] -> String
genHeader mods = concatMap (\i -> "#include <" ++ i ++ ".h>\n") included
    where
        included = nub (concatMap (\(_, _, is, _) -> is) mods)

enumerate :: [a] -> [(Int, a)]
enumerate xs = arf 0 xs
    where
        arf acc (x:xs) = (acc, x):(arf (acc+1) xs)
        arf _ [] = []

getRequired :: [Module] -> [String]
getRequired mods = let (_, _, r, _) = unzip4 mods in concat r

getDeclared :: [Module] -> [String]
getDeclared mods = let (_, d, _, _) = unzip4 mods in concat d

genRenamer :: Files -> Map.Map String String
genRenamer (mods, pg) = let ex = concatMap Map.keys pg in
    Map.fromList (map (\(i, v) -> (v, '_':show i)) (enumerate (ex \\ getDeclared (Map.elems mods))))

genFnName :: [String] -> String -> String
genFnName mods id = (intercalate "_" mods) ++ '_':id

rename :: Map.Map String String -> Identifier -> String
rename map (Global x) = case x `Map.lookup` map of
    Just y -> y
    Nothing -> x
rename map (Module xs x) = genFnName xs x

genTable :: String -> [(String, String)] -> String
genTable tableName names = arf ("uint64_t " ++ tableName ++ "[] = {") names
    where
        arf acc ((old, new):xs) = arf (acc ++ new ++ ",\"" ++ old ++ "\",") xs
        arf acc [] = acc ++ "};\n"

genStack :: String -> String -> Int -> String
genStack stackName ptrName size = "uint64_t " ++ stackName ++ "[" ++ show size ++ "];\nuint64_t " ++ ptrName ++ ";\n"

genBlobs :: [Module] -> String
genBlobs mods = let (_, _, _, bs) = unzip4 mods in concat bs

genDecl :: [String] -> String
genDecl = concatMap (\s -> "void " ++ s ++ "();\n")

genCall :: String -> String
genCall str = str ++ "();\n"

genPush :: String -> String -> String
genPush fn label = fn ++ "(" ++ label ++ ");\n"

data FinalIns = CallIns String | PushIns String

genIns :: String -> FinalIns -> String
genIns _ (CallIns s) = genCall s
genIns l (PushIns s) = genPush l s

genFinalIns :: Map.Map String String -> [Instruction] -> [FinalIns]
genFinalIns namer = map (\s -> case s of
    Push s -> PushIns (rename namer s)
    Call s -> CallIns (rename namer s))

genBody :: Map.Map String String -> String -> [Instruction] -> String
genBody namer label = concatMap (genIns label) . genFinalIns namer

genFn :: Map.Map String String -> String -> String -> [Instruction] -> String
genFn namer label name ins = "void " ++ name ++ "() {\n" ++ (genBody namer label ins) ++ "\n}\n"

generate :: String -> Int -> String -> String -> String -> Files -> String -- TODO: clean up
generate stackName stackSize ptrName pushlabel tableName fs@(mods, progs) =
    let header = genHeader (getModules fs) ++ "#include <stdint.h>\n"
        namer = genRenamer fs
        allIdentifiers = (concatMap (map Global . Map.keys) progs) ++ (concatMap (\(i, _, _, _) -> i) mods)
        printNames = map show allIdentifiers
        fnNames = map (rename namer) allIdentifiers
        table = genTable tableName (zip printNames fnNames)
        stack = genStack stackName ptrName stackSize
        decls = genDecl fnNames
        blobs = genBlobs (Map.elems mods)
        allDefs = (concatMap Map.toList . Map.elems) progs
        renamedDefs = map (\(n, is) -> (rename namer (Global n), is)) allDefs
        defns = concatMap (uncurry (genFn namer pushlabel)) renamedDefs in
            header ++ stack ++ decls ++ table ++ blobs ++ defns