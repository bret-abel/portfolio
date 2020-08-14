{-

  Bret Abel
  CS 291
  11/4/18 - v. 1.3
  Starter code provided by Prof. Brad Richards

  This is a(n evil) hangman game.

-}

import Data.List
import Data.Char
import System.Random

type Words    = [W0rd]
type W0rd     = (Werd, Pattern, Unknowns)
type Werd     = String
type Pattern  = String  -- pattern to match against possible answers
type Unknowns = Int     -- remaining unknown chars

-- Updates the patterns and unknown values based on most recent guess
updateList :: [Char] -> Words -> Words
updateList [] wordList = wordList
updateList guessed []  = []
updateList guessed ((w,knownChars,u):ws)
  |  (head guessed) `elem` w =    ((w,newPat,u') : (updateList guessed ws))
  | otherwise                = ((w,knownChars,u) : (updateList guessed ws))
  where
    newPat = map (\c -> if c `elem` guessed then c else '-') w
    u'     = getUnknowns newPat

cheat :: Words -> Pattern -> (Words, Pattern)
cheat wordList pattern = (newList, newPat)
   where wordPats    = getFams wordList
         wordFams    = getWords wordPats wordList
         bigLength   = maximum (map length wordFams)
         bigFams     = [ fam | fam <- wordFams, length fam == bigLength ]
         maxUnknowns = maximum (map getUnknowns wordPats)
         (best:fams) = filter (\((_,_,u):ws) -> u == maxUnknowns) bigFams
         newList     = best
         newPat      = getPat (head best)
-- get word pattern
getPat :: W0rd -> Pattern
getPat (_,p,_) = p

-- get number of unknown chars
getUnknowns :: Pattern -> Unknowns
getUnknowns p = sum $ (map (\c -> if c == '-' then 1 else 0)) p

getWords :: [Pattern] -> Words -> [Words]
getWords [] _ = []
getWords _ [] = []
getWords (p:ps) wordList =
  (filter (\(w,p',u) -> p' == p) wordList) : (getWords ps wordList)

getFams :: Words -> [Pattern]
getFams []       = []
getFams wordList =
  foldl (\pats p -> if p `elem` pats then pats else pats ++ [p]) [] patterns
  where patterns = [ ps | (_,ps,_) <- wordList ]

-- Run the game
game :: [Char] -> Words -> Pattern -> Int -> IO ()
game guessed wordList knownChars guesses
  |  ('-' `notElem` knownChars)  = do
      putStrLn ("You got it! You guessed the correct word " ++ (show knownChars)
                  ++ " with " ++ (show guesses) ++ " guesses remaining.")
  | guesses == 0 = do
      -- generate and display the "right" answer    
      rand <-  randomRIO (0, (length wordList) - 1)
      let answer = (\(w,_,_) -> w) (wordList !! rand)
      putStrLn ("Sorry, you ran out of guesses!")
      putStrLn ("The correct word was " ++ answer ++ ".")
  | otherwise = do
      {- testing stuff 
      putStrLn ("Under the hood stuff:")
      let wordFams = getFams wordList
      let famWords = getWords wordFams wordList
      putStrLn ("Current word list: " ++ show wordList)
      putStrLn ("Possible word families " ++ show wordFams)
      putStrLn ("Matching Words: " ++ show famWords)
      putStrLn ("")
      -}
      putStrLn ("You have " ++ (show guesses) ++ " guesses remaining.")
      putStrLn ("You have already guessed these letters: " ++ (show guessed))
      putStrLn ("The word so far is: ")
      putStrLn (knownChars)
      putStrLn ("")
      putStrLn ("Please enter your guess: ")
      guessHolder <- getLine
      let guess = head guessHolder
      if guess `elem` guessed then
         do
           putStrLn ("You already guessed that one! Try another.")
           game guessed wordList knownChars guesses
      else
        do
          let guessed'  = (guess:guessed)
              wordList' = updateList guessed' wordList
              shouldNo  = elem knownChars [ getPat w | w <- wordList']
          case shouldNo of
            True  -> do
              putStrLn ("That's not it, try again!")
              let newList = filter (\(w,_,_) -> guess `notElem` w) wordList'
              game guessed' newList knownChars (guesses - 1)
            False -> do
              putStrLn ("Good guess!")
              let (newList, newPat) = cheat wordList' knownChars
              game guessed' newList newPat guesses

-- Run the program
main = do
  let size        = length smallDict
  putStrLn ("The small dictionary contains "++(show size)++" words")
  putStrLn ("Please enter your desired word length:")
  lengthString <- getLine
  putStrLn("Please enter your desired amount of guesses:")
  guessString <- getLine
  let wordLength  = (read lengthString :: Int)
      intlGuesses = (read guessString :: Int)
      dict        = filter (\x -> length x == wordLength) smallDict
      intlPat     = replicate wordLength '-'
      intlList    = [ (w,intlPat,wordLength) | w <- dict ]
  game [] intlList intlPat intlGuesses


-- These words match the ones used in the online writeup of the Evil Hangman
-- assignment.  You can use this dictionary to test the cases shown in the assignment.

trivialDict = ["ally", "beta", "cool", "deal", "else", "flew", "good", "hope", "ibex"]


-- The four-letter words in this dictionary contain only the letters 'e', 'a', 'l',
-- 's', 'r', and 't'.  You can take advantage of the limited character selection to
-- do some testing.

smallDict = ["alae", "alee", "ales", "area", "ares", "arse", "asea", "ates", "earl", "ears", "ease", "east", "eats", "eras", "etas", "lase", "late", "leal", "lear", "leas", "rale", "rare", "rase", "rate", "real", "rear", "sale", "sate", "seal", "sear", "seas", "seat", "sera", "seta", "tael", "tale", "tare", "tate", "teal", "tear", "teas", "tela"]


-- This is a larger group of four-letter words if you want a greater challenge.

mediumDict = ["abbe", "abed", "abet", "able", "abye", "aced", "aces", "ache", "acme", "acne", "acre", "adze", "aeon", "aero", "aery", "aged", "agee", "ager", "ages", "ague", "ahem", "aide", "ajee", "akee", "alae", "alec", "alee", "alef", "ales", "alme", "aloe", "amen", "amie", "anes", "anew", "ante", "aped", "aper", "apes", "apex", "apse", "area", "ares", "arse", "asea", "ates", "aver", "aves", "awed", "awee", "awes", "axed", "axel", "axes", "axle", "ayes", "babe", "bade", "bake", "bale", "bane", "bare", "base", "bate", "bead", "beak", "beam", "bean", "bear", "beat", "beau", "bema", "beta", "blae", "brae", "cade", "cafe", "cage", "cake", "came", "cane", "cape", "care", "case", "cate", "cave", "ceca", "dace", "dale", "dame", "dare", "date", "daze", "dead", "deaf", "deal", "dean", "dear", "deva", "each", "earl", "earn", "ears", "ease", "east", "easy", "eath", "eats", "eaux", "eave", "egad", "egal", "elan", "epha", "eras", "etas", "etna", "exam", "eyas", "eyra", "face", "fade", "fake", "fame", "fane", "fare", "fate", "faze", "feal", "fear", "feat", "feta", "flea", "frae", "gaed", "gaen", "gaes", "gage", "gale", "game", "gane", "gape", "gate", "gave", "gaze", "gear", "geta", "hade", "haed", "haem", "haen", "haes", "haet", "hake", "hale", "hame", "hare", "hate", "have", "haze", "head", "heal", "heap", "hear", "heat", "idea", "ilea", "jade", "jake", "jane", "jape", "jean", "kaes", "kale", "kame", "kane", "keas", "lace", "lade", "lake", "lame", "lane", "lase", "late", "lave", "laze", "lead", "leaf", "leak", "leal", "lean", "leap", "lear", "leas", "leva", "mabe", "mace", "made", "maes", "mage", "make", "male", "mane", "mare", "mate", "maze", "mead", "meal", "mean", "meat", "mesa", "meta", "nabe", "name", "nape", "nave", "neap", "near", "neat", "nema", "odea", "olea", "pace", "page", "pale", "pane", "pare", "pase", "pate", "pave", "peag", "peak", "peal", "pean", "pear", "peas", "peat", "plea", "race", "rage", "rake", "rale", "rape", "rare", "rase", "rate", "rave", "raze", "read", "real", "ream", "reap", "rear", "rhea", "sabe", "sade", "safe", "sage", "sake", "sale", "same", "sane", "sate", "save", "seal", "seam", "sear", "seas", "seat", "sera", "seta", "shea", "spae", "tace", "tael", "take", "tale", "tame", "tape", "tare", "tate", "teak", "teal", "team", "tear", "teas", "teat", "tela", "tepa", "thae", "toea", "twae", "urea", "uvea", "vale", "vane", "vase", "veal", "vela", "vena", "vera", "wade", "waes", "wage", "wake", "wale", "wame", "wane", "ware", "wave", "weak", "weal", "wean", "wear", "weka", "yare", "yeah", "yean", "year", "yeas", "zeal", "zeta", "zoea"]
