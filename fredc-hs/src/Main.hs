module Main where
import Tokenizer
import Parser
import Module
import Cgen

import Control.Monad
import qualified Data.Map as Map
import System.Environment (getArgs)

loadMod :: Files -> String -> IO (Either String Files)
loadMod fs addr = do
    str <- readFile addr
    return $ do
        t <- tokensModule str
        p <- parseFmod t
        m <- makeModule p
        includeModule m addr fs

loadManyMods :: Files -> [String] -> IO (Either String Files)
loadManyMods fs (m:ms) = do
    f <- loadMod fs m
    case f of
        Left err -> return (Left err)
        Right fs' -> loadManyMods fs' ms
loadManyMods fs [] = return (Right fs)

includeFred :: Files -> String -> IO (Either String Files) {- this is fucking unreadable -}
includeFred fs@(mods, progs) addr = do
    s <- readFile addr
    let str = "~std.fmod\n" ++ s
    let fred = do {
        t <- tokens str;
        p <- parseFred t;
        loadWith p fs;
    }
    case fred of
        Left err -> return (Left err)
        Right (Right prog) -> return (Right (mods, Map.insert addr prog progs))
        Right (Left (modnames, prognames)) -> do
            files <- loadManyMods fs modnames
            case files of
                Left err -> (return (Left err))
                Right fs' -> do
                    files' <- includeManyFred fs' prognames
                    case files' of
                        Left err -> (return (Left err))
                        Right fs'' -> includeFred fs'' addr

includeManyFred :: Files -> [String] -> IO (Either String Files)
includeManyFred fs (a:as) = do
    f <- includeFred fs a
    case f of
        Left err -> return (Left err)
        Right fs' -> includeManyFred fs' as
includeManyFred fs [] = return (Right fs)

main :: IO ()
main = do
    args <- getArgs
    case length args of
        2 -> do
            program <- includeFred (Map.empty, Map.empty) (args !! 0)
            case program of
                Left err -> putStrLn err
                Right files -> writeFile (args !! 1) (generate "stack" 65536 "stackptr" "PUSH_LABEL" "labels" files)
        _ -> putStrLn "wrong number of arguments supplied"
