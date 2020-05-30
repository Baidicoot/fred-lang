module Parser (parseFred, parseFmod, Identifier(..), Instruction(..)) where
import Tokenizer (Token(..))
import Prelude hiding(or)
import Data.Maybe (catMaybes)
import Data.List (intercalate)

{- PARSE the tokenstream into pesudo-AST -}

type Identifier = String
data Instruction = Call Identifier | Push Identifier deriving(Show, Eq, Ord)

type Parser a = [Token] -> Either String (a, [Token])

lookahead :: [Token] -> Token
lookahead (x:xs) = x
lookahead [] = TokEnd

err :: [Token] -> String -> Either String a
err xs name = Left ("unexpected " ++ (show (lookahead xs)) ++ " while parsing " ++ name)

parseIdentifier :: Parser Identifier
parseIdentifier (Ident x:xs) = Right (x, xs)
parseIdentifier xs = err xs "identifier"

parseIns :: Parser Instruction
parseIns (Syntax '\'':xs) = case parseIdentifier xs of
    Right (id, xs) -> Right (Push id, xs)
    Left err -> Left err
parseIns xs = case parseIdentifier xs of
    Right (id, xs) -> Right (Call id, xs)
    Left err -> Left err

many :: Parser a -> Parser [a]
many p s = case p s of
    Left _ -> Right ([], s)
    Right (x, s') -> do
        (xs, s'') <- many p s'
        return (x:xs, s'')

or :: Parser a -> Parser a -> Parser a
or a b s = case a s of
    Right (x, xs) -> Right (x, xs)
    Left _ -> b s

before :: Parser a -> Parser b -> Parser b
before a b s = case a s of
    Right (_, s') -> b s'
    Left err -> Left err

sepBy :: Parser a -> Parser b -> Parser [a]
sepBy a b s = do
    (x, s') <- a s
    (xs, s'') <- many (b `before` a) s'
    return (x:xs, s'')

syntax :: Char -> Parser Token
syntax c s@(Syntax d:xs)
    | c == d = Right (Syntax c, xs)
    | otherwise = err s ("syntax '" ++ c:"'")
syntax c s = err s ("syntax '" ++ c:"'")

parseDefn :: Parser (String, [Instruction])
parseDefn (Ident x:Syntax ':':xs) = case many parseIns xs of
    Left err -> Left err
    Right (ids, s) -> Right ((x, ids), s)
parseDefn xs = err xs "definition"

parseField :: Parser (String, [Identifier])
parseField (Ident x:Syntax ':':xs) = case many parseIdentifier xs of
    Left err -> Left err
    Right (ids, s) -> Right ((x, ids), s)
parseField xs = err xs "field"

parseImport :: Parser String
parseImport (Import x:xs) = Right (x, xs)
parseImport xs = err xs "import"

parseModule :: Parser String
parseModule (Builtin x:xs) = Right (x, xs)
parseModule xs = err xs "module import"

parseBlob :: Parser String
parseBlob (Syntax '-':Blob s:[]) = Right (s, [])
parseBlob xs = err xs "blob"

-- fred ::= *((<import> / <include> / <definition>) '\n')
fred :: Parser [(Maybe String, Maybe String, Maybe (String, [Instruction]))]
fred = (mod `or` imp `or` defn) `sepBy` (syntax '\n')
    where
        mod s = case parseModule s of
            Right (x, xs) -> Right ((Just x, Nothing, Nothing), xs)
            Left err -> Left err
        imp s = case parseImport s of
            Right (x, xs) -> Right ((Nothing, Just x, Nothing), xs)
            Left err -> Left err
        defn s = case parseDefn s of
            Right (x, xs) -> Right ((Nothing, Nothing, Just x), xs)
            Left err -> Left err

fmod :: Parser [(String, [Identifier])]
fmod = parseField `sepBy` (syntax '\n')

parseFred :: [Token] -> Either String ([String], [String], [(String, [Instruction])])
parseFred s = do
    (x, xs) <- fred s
    case xs of
        [] -> let (modules, imports, definitions) = unzip3 x in
            Right (catMaybes modules, catMaybes imports, catMaybes definitions)
        _ -> err xs ".fred file"

-- fmod ::= *definition 1*'-' <blob>
parseFmod :: [Token] -> Either String ([(String, [Identifier])], String)
parseFmod s = do
    (x, xs) <- fmod s
    (_, xss) <- many (syntax '\n') xs
    (blob, _) <- parseBlob xss
    return (x, blob)
