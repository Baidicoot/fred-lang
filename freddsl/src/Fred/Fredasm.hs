{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FunctionalDependencies #-}
module Fred.Fredasm where

import Control.Monad
import Control.Monad.State
import Control.Monad.Writer

import qualified Data.Set as Set

-- representation of fred source

data Ident
    = Local String
    | Extern String Ident
    deriving(Eq, Show)

data Ins
    = Quote Ident
    | Call Ident
    deriving(Eq, Show)

data FredFn
    = FredFn Int [Ins]
    deriving(Eq, Show)

-- dsl internals

newtype Procedural
    = Proc (Set.Set Int) -- generated procedures of this form
    deriving(Eq, Show)

type FredState = ([Procedural], Int)
type FredDSL = StateT FredState (Writer [FredFn])

insert :: Procedural -> Int
insert p = do
    ps <- get
    put (ps ++ [p])
    pure (length ps)

hasProc :: Int -> FredDSL Bool
hasProc i = do
    (xs, _) <- get
    pure (i < length xs)

defun :: 

getInst :: Int -> Int -> (Int -> FredDSL ()) -> FredDSL (Maybe Int)
getInst id i fn = do


procedure :: Int -> (Int -> FredDSL ()) -> Int -> FredDSL ()
procedure id fn i = do
    b <- hasProc id
    if b then $ do
        m <- id `getInst` i
        case m of

    else

{-
data Var t
    = Var Int
    deriving(Eq, Show)

class Value x t | x -> t where
    store :: x -> FredDSL (Var t)

instance Store Int Int where
    store i = do
        id <- varID
-}