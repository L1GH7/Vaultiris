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

use Symfony\Component\Finder\Finder;

include(dirname(__FILE__).'/../../config/config.inc.php');
include(dirname(__FILE__).'/vendor/autoload.php');

$token = Tools::getValue('token');
$action = Tools::getValue('action');
$security_token = Configuration::get('LBLCVE_TOKEN');

if ($token == $security_token) {
    // Variables
    $files_pattern = array();
    $files_names = array();
    $files_path_patch = array();
    $message = null;

    // Check for every file to be patched
    $from_where = dirname(__FILE__).'/diff/';
    $files_type = '*.pattern';

    // Protection against not affected versions
    if (version_compare(_PS_VERSION_, '21.7.6.3', '<='))
    {
        $finder = new Finder();
        $finder->files()->name($files_type)->in($from_where);
            
        // Search for pattern files
        if ($finder->hasResults()) {
            foreach ($finder as $file) {
                $files_pattern[] = $file->getRealPath();
                $files_names[] = $file->getFilename();
                $first_line = file($file->getRealPath());
                $files_path_patch[] = str_replace(PHP_EOL, "", $first_line[0]);
            }
        }
    }     

    switch($action)
    {
        case 'patch':
            include(dirname(__FILE__).'/classes/LblPatcher.php');
            $patcher = new LblPatcher();
            $result = $patcher->patchFiles($files_pattern, $files_names, $files_path_patch);
            $message =  array(
                'message' => 'Patch process finished.',
                'files' => $result['patched'],
                'names' => $result['unpatched'],
                'path' => $result['original']
            );            
        break;
        case 'list_patches':
            $message =  array(
                'message' => 'Ready and waiting...',
                'files' => $files_pattern,
                'names' => $files_names,
                'path' => $files_path_patch,
                'patches' => count($files_names)
            );
        break;
    }

    die(json_encode($message));
}

?>