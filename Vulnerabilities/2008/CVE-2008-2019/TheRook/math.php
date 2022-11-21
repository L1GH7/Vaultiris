<?php
/* ETC.INC contains definitions for the SubMatrix and SeqMatch classes.
   It also contains helper functions such as is_blank(), isa_qualifier(), 
   firstchar(), left(), etc. */

$patterndb = array("_StartCodon" => "AUG", "_EndCodon" => "[UAA,UAG,UGA]");

// trim_element() removes leading and trailing spaces from a string.  In conjunction 
// with the array_walk() function, it removes spaces from each element of an array.
function trim_element(&$value, $key)
   {
   $value = trim($value);
   }


/* 
is_false() tests if a value is a boolean false and not a zero (0).  This is necessary 
to correctly interpret the return value of some PHP functions like strpos().  strpos()
returns a zero (0) if a string is found at the beginning of a larger string, and FALSE
if it cannot find that string within the larger string.  In PHP, FALSE equals 0. 
*/
function is_false($value)
   {
   if ( (gettype($value) == "boolean") and
      ($value == FALSE) ) return TRUE;
   else return FALSE;
   }

// rem_right() removes $charcount characters from the right (end) of a string.
function rem_right($str, $charcount = 1)
   {
   return substr($str, 0, strlen($str)-$charcount);
   }

// intrim() removes "internal spaces" (as opposed to leading and trailing spaces) from a string.
function intrim($string)
   {
   return eregi_replace(' ', '', $string);
   }

// getmin() gets the minimum of three (usually numeric) values $x, $y, and $z.
// For now, this can't handle situations when one or more arguments is FALSE.
function getmin($x, $y, $z)
   {
   if ($x < $y)
      if ($x < $z) return $x;
      else return $z;
   else
      if ($y < $z) return $y;
      else return $z;
   }

// is_even() tests if an integer is an even number.
function is_even($integer)
   {
   if (($integer/2) == ((int) ($integer/2))) return TRUE;
   else return FALSE;
   }

// is_odd() tests if an integer is an odd number.  This is the opposite of is_even().
function is_odd($integer)
   {
   if (($integer/2) != ((int) ($integer/2))) return TRUE;
   else return FALSE;
   }

// is_blankstr() tests if a value is a blank string ("").  Like is_false(), this
// helps interpret the value of some PHP functions or expressions.
function is_blankstr($var)
   {
   if ( (gettype($var) == "string") and ($var == "") ) return TRUE;
   else return FALSE;
   }

// I think this function should give way to or be replaced by is_blankstr().
// I haven't removed this yet as I have to check if some code still uses it.
function is_blank($str)
   {
   if ($str == "") return true;
   else return false;
   }

// firstchar() returns the first or beginning character of a string.
function firstchar($str)
   {
   return left($str, 1);
   }

// left() returns the first $numchars characters of a string.
function left($str, $numchars)
   {
   return substr($str, 0, $numchars);
   }

// right() returns the substring beginning at $numchars characters from the right end of a string.
function right($str, $numchars)
   {
   return substr($str, strlen($str)-$numchars);
   }

// compare_letter() compares two letters $let1 and $let2 and returns another letter
// indicating if the two were exact matches, partial matches, or non-matches.
function compare_letter($let1, $let2, $matrix, $equal, $partial = "+", $nomatch = ".")
	{
	global $chemgrp_matrix;

	// if no custom substitution matrix was provided, use the default.
	if (isset($matrix) == FALSE) $matrix = $chemgrp_matrix->rules;
	
	// if no symbol for exact matches was provided, use the residue symbol.
	if (isset($equal) == FALSE) $equal = $let1;
	
	if ($let1 == $let2) return $equal;
	elseif (partial_match($let1, $let2, $matrix)) return $partial;
	else return $nomatch;
	}


/* Algorithm:
   We abbreviate substitution matrix to "submatrix".  Each element in a submatrix is an array of
   symbols that are considered "partial matches" of each other.
	
   Default submatrix: ( (G,A,V,L,I), (S,T), (N,Q), (F,Y,W), (C, M), (P), (D,E), (K,R,H), (*), (X) )

   1) Check if both $let1 and $let2 appear in the first element (G,A,V,L,I) of the substitution matrix.

   2) If they are, you've found a "hit", and $let1 and $let2 are partial matches.  Return a TRUE value.
      If they are not, then go to the next element in the substitution matrix.  
		
      Repeat steps 1 and 2 until you reach a submatrix element where both $let1 and $let2 appear, or 
      until the last element in the submatrix has been checked.
		
   3) If you reach the last submatrix element without a "hit", return a FALSE value.
	
   NOTE: This will not warn if you $let1 and/or $let2 is nowhere to be found in the whole submatrix.		
*/
function partial_match($let1, $let2, $matrix)
   {
   global $chemgrp_matrix;
   if (isset($matrix) == FALSE) $matrix = $chemgrp_matrix->rules;
   foreach($matrix as $rule)
      if ((in_array($let1, $rule)) and (in_array($let2, $rule))) return TRUE;
   return FALSE;
   }

// getpattern() retrieves the pattern string from the pattern database ($patternDB array).
function getpattern($pattern)
   {
   global $patterndb;
   return $patterndb[$pattern];
   }

// This class allows the use of customized substitution matrices.  See tech doc for details.
class SubMatrix
{
var $rules;

// submatrix simply initializes the rules property to the empty array.
function SubMatrix()
   {
   $this->rules = array();
   }

// addrule() adds a rule to the substitution matrix.
function addrule($x)
   {
   $x = func_get_args();
   // if (isset($this->rules) == FALSE) $this->rules = array();
   array_push($this->rules, $x);
   }
}

class SeqMatch
{
var $result;
var $hamdist;
var $levdist;

// hamdist() computes the Hamming Distance between two strings or Seq objects 
// of equal length.  For more information, consult the technical reference.

function hamdist($seq1, $seq2)
   {
   // If $seq1 is a Seq object, we use its sequence property to compute Hamming Distance.
   if (gettype($seq1) == "object") $string1 = $seq1->sequence;
   elseif (gettype($seq1) == "string") $string1 = $seq1;

   // If $seq2 is a Seq object, we use its sequence property to compute Hamming Distance.
   if (gettype($seq2) == "object") $string2 = $seq2->sequence;
   elseif (gettype($seq2) == "string") $string2 = $seq2;

   // We terminate code execution if the two strings differ in length.
   if (strlen($string1) != strlen($string2))
      die("Both sequence must be of the same length!");

   $len = strlen($string1);
   // Initialize the hamming distance to 0 (no difference between two strings).
   $distance = 0;

   // Match the two strings, character by character.  If they are NOT
   // identical, increment $distance by 1.
   for($i = 0; $i < $len; $i++)
      {
      $let1 = substr($string1, $i, 1);
      $let2 = substr($string2, $i, 1);
      if ($let1 != $let2) $distance++;
      }
   return $distance;
   }

// levdist() computes the Levenshtein Distance between two strings or Seq objects 
// with equal/unequal lengths.  You can pass custom values for cost of insertion,
// replacement, and deletion.  If you don't pass any, they are assumed to be 1.
// For more information, see technical reference.

function levdist($seq1, $seq2, $cost_ins = 1, $cost_rep = 1, $cost_del = 1)
   {
   // If $seq1 is a Seq object, we use its sequence property to compute Levenshtein Distance.
   if (gettype($seq1) == "object") $string1 = $seq1->sequence;
   elseif (gettype($seq1) == "string") $string1 = $seq1;

   // If $seq2 is a Seq object, we use its sequence property to compute Levenshtein Distance.
   if (gettype($seq2) == "object") $string2 = $seq2->sequence;
   elseif (gettype($seq2) == "string") $string2 = $seq2;

   // Check the lengths of the two strings.  If they exceed 255 characters, terminate code.
   if (strlen($string1) > 255) die("String length must not exceed 255 characters!");
   if (strlen($string2) > 255) die("String length must not exceed 255 characters!");

   // Compute and return the Levenshtein Distance using PHP's built-in levenshtein() function.
   return levenshtein($string1, $string2, $cost_ins, $cost_rep, $cost_del);
   }

// xlevdist() is an extended version of levdist() which accepts strings with length
// greater than 255 but not to exceed 1024 (which takes my CPU 18 seconds to compute).
// The only drawback to xlevdist is that the cost of insertion, deletion, and replacement
// is fixed to 1.  I have yet to find a way to allow custom values for these.

function xlevdist($s, $t)
   {
   $n = strlen($s);
   $m = strlen($t);

   if (($n > 1024) or ($m > 1024)) die("String length must not exceed 1024 characters");

   // initialize the array
   $values = array();
   $temp = array();
   $temp[0] = 0;

   for($j = 1; $j <= $m; $j++)
      $temp[$j] = 0;

   $values[0] = $temp;
   for($i = 1; $i <= $n; $i++)
      $values[$i] = $temp;

   for($i = 1; $i <= $n; $i++)
      {
      $lets = substr($s, $i-1, 1);
      for($j = 1; $j <= $m; $j++)
         {
         $lett = substr($t, $j-1, 1);
         if ($lets == $lett) $cost = 0;
         else $cost = 1;

         // "normal" values of $up, $left, and $upleft
         if ($j > 1) $up = $values[$i][$j-1];
         else $up = FALSE;
         if ($i > 1) $left = $values[$i-1][$j];
         else $left = FALSE;
         if (($i > 1) and ($j > 1)) $upleft = $values[$i-1][$j-1];
         else $upleft = FALSE;

         if ($i == 1)
            {
            if ($j == 1) $value = $cost;
            elseif ($cost == 0) $value = $cost;
            else $value = $up + 1;
            }
         else
            {
            // if at the first or topmost row, there is no upleft and above.
            if ($j == 1)
               {
               if ($cost == 0) $value = $cost;
               else $value = $left + 1;
               }
            else $value = getmin($up + 1, $left + 1, $upleft + $cost);
            }
         $values[$i][$j] = $value;
         } // closes for($j ...
      } // closes for($i ...
      return $values[$n][$m];
   } // closes function xlevdist()

/*
The match() method accepts two sequence strings (not objects) of equal length,
and returns a sequence match result string, according to the following rules:

  If there is an exact match, return the amino acid symbol.
  If there is a partial match, return a plus sign.
  If there is no match, return a whitespace character.
*/

function match($str1, $str2, $matrix, $equal, $partial = "+", $nomatch = ".")
   {
   global $chemgrp_matrix;

   // if the user chose not to use a custom submatrix, use the default one.
   if (isset($matrix) == FALSE) $matrix = $chemgrp_matrix->rules;
	
   // if the strings differ in length, terminate code execution.
   if (strlen($str1) != strlen($str2))
      die("Cannot match sequences with unequal lengths");
   $resultstr = "";
   $seqlength = strlen($str1);
	
   // Match the two strings, character by character.  Each call to compare_letter()
   // function returns a "result character" which is appended to a "result string".
   for($i = 0; $i < $seqlength; $i++)
      {
      $let1 = substr($str1, $i, 1);
      $let2 = substr($str2, $i, 1);
      $resultstr = $resultstr . compare_letter($let1, $let2, $matrix, $equal, $partial, $nomatch);
      }
		
   // Assign "result string" to the result property of the calling SeqMatch object. 
   $this->result = $resultstr;
	
   // Return the result string.  While this line and the line above seems redundant, their
   // presence here actually permits programmers to write more compact code.
   return $resultstr;
   }
}
?>