main :: IO String; main = do { putStrLn "begin"; n <- randomInt; text <- readScript "bigint_sum.lisp"; return text };
