<?php
/**
* 2020 Labelgrup
*
* NOTICE OF LICENSE
*
* READ ATTACHED LICENSE.TXT
*
*  @author    Manel Alonso <malonso@labelgrup.com>
*  @copyright 2020 Labelgrup
*  @license   LICENSE.TXT
*/

if (!defined('_PS_VERSION_')) {
    exit;
}

//include(dirname(__FILE__).'/../vendor/autoload.php');

//use Spatie\Regex\Regex;

class LblPatcher
{
    private $patched_files = array();
    private $non_patched_files = array();
    
    /*
     * Method to patch files
     */
    public function patchFiles($files_pattern, $files_name, $files_path)
    {
        foreach ($files_pattern as $index => $pattern_file) {
            $original_file_name = str_replace(".pattern", ".php", $files_name[$index]);
            $patch_file = str_replace(".pattern", ".patch", $pattern_file);
            $text_to_find = $this->stripFirstLine(Tools::file_get_contents($pattern_file));
            $text_to_rplc = $this->stripFirstLine(Tools::file_get_contents($patch_file));
            $path_file = dirname(__FILE__).'/../../..'.$files_path[$index].$original_file_name;
            // Open the original file
            $original_file_text = Tools::file_get_contents($path_file);
            // TODO: foreach lines to patch
            if (strpos($original_file_text, $text_to_find) !== false)
            {
                // Found a coincidence
                str_replace($text_to_find, $text_to_rplc, $original_file_text);
                $new_content = $original_file_text;
            }

        }
        return null;
    }

    /**
     * Removes the first line from a text (where the file path is)
     */
    private function stripFirstLine($text)
    {
        return substr($text, strpos($text, "\n") + 1);
    }    
}
