module Tokenizer (tokens, tokensModule, Token(..)) where
import Data.Char
import Prelude hiding(until)

{- TOKENIZES the input stream for parsing -}

data Token = Import String | Builtin String | Syntax Char | Ident String | Blob String | TokEnd deriving(Show, Eq)

type Lexer a = String -> (a, String)

until :: (Char -> Bool) -> Lexer String
until fn (x:xs) =
    if fn x then
        ([], x:xs)
    else
        let (a, str) = until fn xs in
            (x:a, str)
until _ [] = ([], [])

while :: (Char -> Bool) -> Lexer String
while fn (x:xs) =
    if fn x then
        let (a, str) = while fn xs in
            (x:a, str)
    else
        ([], x:xs)
while _ [] = ([], [])

rest :: Lexer String
rest str = (str, [])

before :: Lexer a -> Lexer b -> Lexer b
before a b str = let (_, rem) = a str in b rem

ident :: Lexer String
ident = until (\c -> isSpace c || c `elem` ":.'")

whitespace :: Lexer String
whitespace = while isSpace

-- lexer for .fred files, maybe
tokenize :: Lexer [Token]
tokenize s = arf [] s
    where
        arf toks s@(x:xs) {- generally filter out special characters -}
            | x == '@' = let (file, rem) = until isSpace xs in
                arf (Import file:toks) rem
            | x == '~' = let (file, rem) = until isSpace xs in
                arf (Builtin file:toks) rem
            | x == '\'' = arf (Syntax '\'':toks) xs
            | x == ':' = arf (Syntax ':':toks) xs
            | x == '.' = arf (Syntax '.':toks) xs
            | x == '\n' = let (_, xs') = whitespace xs in
                arf (Syntax '\n':toks) xs'
            | isSpace x = arf toks xs
            | otherwise = let (id, rem) = ident s in
                arf (Ident id:toks) rem
        arf acc [] = (acc, [])

tokens :: String -> Either String [Token]
tokens s = let (toks, str) = tokenize s in case str of
    [] -> Right (reverse toks)
    _ -> Left "did not consume entire stream"

tokenizeModule :: Lexer [Token]
tokenizeModule s = arf [] s
    where
        arf toks s@(x:xs)
            | x == ':' = arf (Syntax ':':toks) xs
            | x == '.' = arf (Syntax '.':toks) xs
            | x == '\n' = let (_, xs') = whitespace xs in
                arf (Syntax '\n':toks) xs'
            | x == '-' = let (str, _) = ((while (== '-')) `before` rest) s in
                (Blob str:Syntax '-':toks, [])
            | isSpace x = arf toks xs
            | otherwise = let (id, rem) = ident s in
                arf (Ident id:toks) rem
        arf acc [] = (acc, [])

tokensModule :: String -> Either String [Token]
tokensModule s = let (toks, str) = tokenizeModule s in case str of
    [] -> Right (reverse toks)
    _ -> Left "did not consume entire stream"