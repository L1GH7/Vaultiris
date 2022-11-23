(ns bcrypt-test
  (:require [clojure.test.check.generators :as gen]
            [clojure.test.check.properties :as prop]
            [clojure.test.check.clojure-test :refer [defspec]])
  (:import (org.bouncycastle.crypto.generators OpenBSDBCrypt)))

(def cost-factor 4)

(def gen-salt
  (gen/fmap byte-array (gen/vector gen/byte 16)))

(def gen-passwords
  (->> (apply gen/tuple (repeat 2 gen/string-alphanumeric))
       (gen/such-that (fn [[right wrong]] (not= right wrong)))))

(def wrong-password-should-fail-prop
  (prop/for-all [[right wrong] gen-passwords
                 salt gen-salt]
    (let [pwhash (OpenBSDBCrypt/generate (.toCharArray right) salt cost-factor)]
      (and (OpenBSDBCrypt/checkPassword pwhash (.toCharArray right))
           (not (OpenBSDBCrypt/checkPassword pwhash (.toCharArray wrong)))))))

(defspec wrong-password-test 1000 wrong-password-should-fail-prop)

(comment
  (require '[clojure.test.check :as tcheck])
  (tcheck/quick-check 1000 wrong-password-should-fail-prop)

  {:shrunk
   {:smallest
    [["" "0"] [0 0 0 2 0 0 0 -19 -16 -25 -24 -5 53 15 36 -88]]
    ,,,}
   :num-tests 348
   :seed 1608411010625
   :pass? false
   :fail
   [["8WD05XzEZVWgbR"
     "80452s5aSB9Sa1F0XpZGk8LNKoSmO14ZX0NSQVy1mxv31ifDfu2uaR6X1RVJvssphlKhKCNoF3drX5fI460c7GALuCNPZN1MiGQ51GsIEBWe20N86LAeg7L2"]
    [-85 -19 -1 11 1 -8 67 -37 -53 -25 -47 -5 53 15 36 -88]]
   ,,,}
  )
