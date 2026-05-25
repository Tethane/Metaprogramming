data MaybeInt = Nothing | Just Int; fromMaybe :: Int -> MaybeInt -> Int; fromMaybe d m = case m of { Nothing -> d; Just x -> x }; main :: Int; main = fromMaybe 7 (Just 42);
