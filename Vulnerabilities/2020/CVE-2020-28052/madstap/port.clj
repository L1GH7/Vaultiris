(ns port
  "Pretty much exactly the logic from the java test,
  but as a generative test in clojure.

  https://github.com/bcgit/bc-java/commit/97578f9b7ed277e6ecb58834e85e3d18385a4219#diff-60a5e3ff66dd8fb8000244e246c0e2d6db9c2b8e2baedd7fda9eb5a215dee28f"
  (:require [clojure.test.check.generators :as gen]
            [clojure.test.check.properties :as prop]
            [clojure.test.check.clojure-test :refer [defspec]])
  (:import (org.bouncycastle.crypto.generators OpenBSDBCrypt)))

(def cost-factor 4)

(def right (.toCharArray "test-token"))
(def wrong (.toCharArray "wrong-token"))

(def gen-salt
  (gen/fmap byte-array (gen/vector gen/byte 16)))

(def wrong-password-should-fail-prop
  (prop/for-all [salt gen-salt]
    (let [pwhash (OpenBSDBCrypt/generate right salt cost-factor)]
      (and (OpenBSDBCrypt/checkPassword pwhash right)
           (not (OpenBSDBCrypt/checkPassword pwhash wrong))))))

(defspec wrong-password-test 1000
  wrong-password-should-fail-prop)
