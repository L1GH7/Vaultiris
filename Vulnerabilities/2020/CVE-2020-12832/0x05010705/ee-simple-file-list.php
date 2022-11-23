<?php

/**
 * @package Element Engage - Simple File List
 */
/*
Plugin Name: Simple File List
Plugin URI: http://simplefilelist.com
Description: A Basic File List Manager | <a href="https://simplefilelist.com/donations/simple-file-list-project/">Donate</a> | <a href="admin.php?page=ee-simple-file-list&tab=pro">Get Pro</a>
Author: Mitchell Bennis
Version: 4.2.7
Author URI: http://simplefilelist.com
License: GPLv2 or later
Text Domain: ee-simple-file-list
Domain Path: /languages
*/

$eeSFL_DevMode = FALSE; // Enables visible logging

if ( ! defined( 'ABSPATH' ) ) exit; // Exit if accessed directly

// SFL Versions

define('eeSFL_Version', '4.2.7'); // Plugin version - DON'T FORGET TO UPDATE ABOVE TOO !!!
define('eeSFL_DB_Version', '4.2'); // Database structure version - used for eeSFL_VersionCheck()
define('eeSFL_Cache_Version', '7'); // Cache-Buster version for static files - used when updating CSS/JS

// Our Core
$eeSFL = FALSE; // Our main class
$eeSFL_ID = 1;
$eeSFL_Settings = array(); // All List Info
$eeSFL_Config = array(); // This List Info
$eeSFL_Env = array(); // Environment
$eeSFL_ListRun = 1; // Count of lists per page
$eeSFL_UploadFormRun = FALSE; // Check if uploader form has run

// The Log - Written to wp_option -> eeSFL-Log
$eeSFL_Log = array('Simple File List is Loading...');
$eeSFL_Log[] = 'Version ' . eeSFL_Version . ' (DB ' . eeSFL_DB_Version . ')';
$eeSFL_Log[] = 'ABSPATH: ' . ABSPATH;
// Format: [] => 'log entry'
//	['messages'][] = 'Message to the user'
//	['errors'][] = 'Error condition to user'

// Supported Extensions
$eeSFL_Extensions = array( // Slugs
	'ee-simple-file-list-folders' // Folder Support
	,'ee-simple-file-list-search' // Search & Pagination
);
$eeSFLF = FALSE; $eeSFLS = FALSE;
$eeSFLF_ListFolder = FALSE;

// simplefilelist_upload_job <<<----- File Upload Action Hooks (Ajax)
add_action( 'wp_ajax_simplefilelist_upload_job', 'simplefilelist_upload_job' );
add_action( 'wp_ajax_nopriv_simplefilelist_upload_job', 'simplefilelist_upload_job' );

// simplefilelist_edit_job <<<----- File Edit Action Hooks (Ajax)
add_action( 'wp_ajax_simplefilelist_edit_job', 'simplefilelist_edit_job' );
add_action( 'wp_ajax_nopriv_simplefilelist_edit_job', 'simplefilelist_edit_job' );


// Plugin Setup
function eeSFL_Setup() {
	
	global $eeSFL, $eeSFL_ID, $eeSFL_Extensions, $eeSFL_Log, $eeSFL_Config, $eeSFL_Settings, $eeSFL_Env;
	
	$eeSFL_Log[] = 'Running Setup...';

	// Get Functions
	$eeSFL_Nonce = wp_create_nonce('eeSFL_Functions'); // Security
	include_once(plugin_dir_path(__FILE__) . 'includes/ee-functions.php'); // Our Functions
	
	// Get Class
	if(!class_exists('eeSFL')) {
		$eeSFL_Nonce = wp_create_nonce('eeSFL_Class'); // Security
		require_once(plugin_dir_path(__FILE__) . 'includes/ee-class.php'); // Get the main class file
		$eeSFL = new eeSFL_MainClass(); // Initiate the SFL Class
		$eeSFL_Env = $eeSFL->eeSFL_GetEnv(); // Get the Environment Array
		
		eeSFL_VersionCheck(); // Update database if needed.
		
		if( @$_REQUEST['eeListID'] ) {
			$eeSFL_ID = filter_var($_REQUEST['eeListID'], FILTER_VALIDATE_INT);
			if( !$eeSFL_ID ) { $eeSFL_ID = 1; } // Default to main list
		}
		
		// Get the lists
		$eeSFL_Settings = get_option('eeSFL-Settings');
		$eeSFL_Config = $eeSFL->eeSFL_Config($eeSFL_ID); // Get this list
	}	
	
	// If Sending Files
	if(@$_POST['eeSFL_Send']) { $eeSFL->eeSFL_SendFilesEmail(); }
	
	// Check/Create the Upload Folder
	if( !eeSFL_FileListDirCheck( $eeSFL_Config['FileListDir'] ) ) { 
		$eeSFL_Log['errors'][] = 'The upload directory is acting up.';
	}
	
	// Extension Checks ------------------------
	
	// A required resource...
	if(!function_exists('is_plugin_active')) {
		include_once( ABSPATH . 'wp-admin/includes/plugin.php' ); 
	}
	
	$eeSFL_Log[] = 'Checking for Extensions...';
	
	// Loop thru and set up
	foreach($eeSFL_Extensions as $key => $eeSFL_Extension) {
	
		if(file_exists(WP_PLUGIN_DIR . '/' . $eeSFL_Extension . '/ee-ini.php')) { // Is the extension present?
	
			if(!file_exists(WP_PLUGIN_DIR . '/' . $eeSFL_Extension . '/ee-check.txt')) {
				
				$eeSFLF_ERROR = '<strong>' . $eeSFL_Extension . ' &larr; ' . __('EXTENSION DISABLED', 'ee-simple-file-list') . '</strong><br />' . 
					__('Please go to Plugins and update the extension to the latest version.', 'ee-simple-file-list');
				
				if( is_admin() AND @$_GET['page'] == 'ee-simple-file-list') {
					$eeSFL_Log['errors'][] = $eeSFLF_ERROR;
				}
				
				continue;
			}
			
			$eeSFL_Env['installed'][] = $eeSFL_Extension;
			
			if(is_plugin_active( $eeSFL_Extension . '/' . $eeSFL_Extension . '.php' )) { // Is the plugin active?
			
				$eeSFL_Log['active'][] = $eeSFL_Extension;
				
				$eeSFL_Nonce = wp_create_nonce('eeSFL_Include'); // Used in all extension INI files
				
				include_once(WP_PLUGIN_DIR . '/' . $eeSFL_Extension . '/ee-ini.php'); // Run initialization
			}
		}
	}
	
	eeSFL_Textdomain(); // Language Setup
	
	return TRUE;
}
add_action('init', 'eeSFL_Setup');



// Custom Hook
function eeSFL_UploadCompleted() {
    do_action('eeSFL_UploadCompleted'); // To be fired post-upload
}

function eeSFL_UploadCompletedAdmin() {
    do_action('eeSFL_UploadCompletedAdmin'); // To be fired post-upload
}


// Log Failed Emails
function eeSFL_action_wp_mail_failed($wp_error) {
    
    global $eeSFL_Log;
    
    $eeSFL_Log['Email-Fail'] = $wp_error;
    
    return error_log(print_r($wp_error, true));
}
add_action('wp_mail_failed', 'eeSFL_action_wp_mail_failed', 10, 1);



// Language Enabler
function eeSFL_Textdomain() {
    load_plugin_textdomain( 'ee-simple-file-list', FALSE, basename( dirname( __FILE__ ) ) . '/languages/' );
}


// Createing a New Post with Shortcode
function eeSFL_CreatePostwithShortcode() { 
	
	if(@$_POST['eeCreatePostType']) {
		
		global $eeSFL_Log;
		
		$eeSFL_ID = FALSE;
		$eeShortcode = FALSE;
		$eeCreatePostType = FALSE;
		$eePostTitle = FALSE;
		
		// eeSFLA
		$eeSFL_ID = filter_var( @$_REQUEST['eeListID'], FILTER_VALIDATE_INT);
		if(!$eeSFL_ID) {  $eeSFL_ID = filter_var( @$_REQUEST['eeNewListID'], FILTER_VALIDATE_INT); } 
		if(!$eeSFL_ID) {  $eeSFL_ID = 1; }
		
		$eeCreatePostType = sanitize_text_field(@$_POST['eeCreatePostType']);
		$eeShortcode = sanitize_text_field(@$_POST['eeShortcode']);
		$eePostTitle = sanitize_text_field(@$_POST['eePostTitle']);
		
		if(!$eeShortcode) {
			$eeShortcode = '[eeSFL list="' . $eeSFL_ID . '"]';
		}
		
		if(!$eePostTitle) {
			$eePostTitle = 'My Simple File List ' . $eeCreatePostType;
		}
			
		if(($eeCreatePostType == "Post" OR $eeCreatePostType == "Page") AND $eeShortcode) {
			
			// Create Post Object
			$eeNewPost = array(
				'post_type'		=> $eeCreatePostType,
				'post_title'    => $eePostTitle,
				'post_content'  => '<div>' . $eeShortcode . '</div>',
				'post_status'   => 'draft'
			);
			
			$eeSFL_Log['Creating Post'] = $eeNewPost;
	 
			// Create Post
			$eeNewPostID = wp_insert_post( $eeNewPost );
			
			if($eeNewPostID) {
				
				$eeSFL_Log['p=' . $eeNewPostID][] = 'Creating new ' . $eeCreatePostType . ' with shortcode...';
				$eeSFL_Log['p=' . $eeNewPostID][] = $eeShortcode;
				
				header('Location: /?p=' . $eeNewPostID);
			}
			
			return TRUE;
		}
	}
}
add_action( 'wp_loaded', 'eeSFL_CreatePostwithShortcode' );



// Shortcode
function eeSFL_Shortcode($atts, $content = null) {
	
	// Basic Usage: [eeSFL]
    
    global $eeSFL, $eeSFL_ID, $eeSFL_DevMode, $eeSFL_Log, $eeSFL_Env, $eeSFL_Settings, $eeSFL_Config, $eeSFL_ListRun, $eeSFL_UploadFormRun;
    global $eeSFLF, $eeSFLS; // Extensions
	
	$eeAdmin = is_admin();
	if($eeAdmin) { return FALSE; } // Don't execute shortcode on page editor
    
    $eeSFL_ListNumber = $eeSFL_ListRun; // Legacy 03/20
    $eeForceSort = FALSE;
    $eeSFLF_ShortcodeFolder = FALSE;
	
	$eeOutput = '';
	
	$eeSFL_Log['L' . $eeSFL_ListRun][] = 'Shortcode Loading: ' . get_permalink();

	if( $eeSFL_ListRun > 1 AND @$_GET['eeFront'] ) { return; }

    // Over-Riding Shortcode Attributes
	if($atts) {
	
		$atts = shortcode_atts( array( // Use lowercase att names only
			'showlist' => '', // YES, ADMIN, USER or NO
			'allowuploads' => '', // YES, ADMIN, USER or NO
			'showthumb' => '', // YES or NO
			'showdate' => '', // YES or NO
			'showsize' => '', // YES or NO
			'showheader' => '', // YES or NO
			'showactions' => '', // YES or NO
			'sortby' => '', // Name, Date, Size, or Random
			'sortorder' => '', // Descending or Ascending
			'hidetype' => '', // Hide file types
			'hidename' => '', // Hide the name matches
			'showfolder' => '', // eeSFLF
			'paged' => '', // eeSFLS
			'filecount' => '', // eeSFLS
			'search' => '', // eeSFLS
			'matchrole' => '' // eeSFLA
		), $atts );
		
		extract($atts);
	
		$eeSFL_Log['L' . $eeSFL_ListRun][] = 'Shortcode Attributes...';
		
		if($showlist) { $eeSFL_Config['ShowList'] = $showlist; }
		if($allowuploads) { $eeSFL_Config['AllowUploads'] = $allowuploads; }
		if($showthumb) { $eeSFL_Config['ShowFileThumb'] = $showthumb; }
		if($showdate) { $eeSFL_Config['ShowFileDate'] = $showdate; }
		if($showsize) { $eeSFL_Config['ShowFileSize'] = $showsize; }
		if($showheader) { $eeSFL_Config['ShowHeader'] = $showheader; }
		if($showactions) { $eeSFL_Config['ShowFileActions'] = $showactions; }
		
		
		if($sortby OR $sortorder) { // Force a re-sort of the file list array if a shortcode attribute was used
			if( $sortby != $eeSFL_Config['SortBy'] OR $sortorder != $eeSFL_Config['SortOrder'] ) {
				$eeForceSort = TRUE;
				$eeSFL_Config['SortBy'] = $sortby;
				$eeSFL_Config['SortOrder'] = $sortorder;
			} else {
				$eeForceSort = FALSE;
			}
		}
		
		if($hidetype) { $eeSFL_HideType = strtolower($hidetype); } else { $eeSFL_HideType = FALSE; }
		if($hidename) { $eeSFL_HideName = strtolower($hidename); } else { $eeSFL_HideName = FALSE; }
		
		// Useless without eeSFLF
		$eeSFLF_ShortcodeFolder = $showfolder;
		
		// Useless without eeSFLS
		if($paged) { $eeSFL_Config['EnablePagination'] = $paged; }
		if($filecount) { $eeSFL_Config['FilesPerPage'] = $filecount; }
		if($search) { $eeSFL_Config['EnableSearch'] = $search; }
		
		// Useless without eeSFLA
		if($matchrole) { $eeSFL_Config['MatchRole'] = $matchrole; }

		
	} else {
		$eeSFL_Log['L' . $eeSFL_ListRun][] = 'No Shortcode Attributes';
	}
	
	
	// Begin Front-Side List Display ==================================================================
	
	$eeOutput .= '<div class="eeSFL"';
	
	if($eeSFL_ListRun == 1) {$eeOutput .= ' id="eeSFL"'; } // 3/20 - Legacy for user CSS
	
	$eeOutput .= '>';
	
	// if($eeSFLF AND $eeSFL_DevMode) { $eeOutput .= 'Folder: ' . $eeSFLF_ShortcodeFolder; }
	
	// Upload Check
	$eeSFL_Nonce = wp_create_nonce('eeInclude'); // Security
	include(WP_PLUGIN_DIR . '/' . $eeSFL->eePluginNameSlug . '/includes/ee-upload-check.php');
	
	// Who Can Upload?
	switch ($eeSFL_Config['AllowUploads']) {
	    case 'YES':
	        break; // Show It
	    case 'USER':
	        // Show It If...
	        if( get_current_user_id() ) { break; } else { $eeSFL_Config['AllowUploads'] = 'NO'; }
	    case 'ADMIN':
	        // Show It If...
	        if(current_user_can('manage_options')) { break; } else { $eeSFL_Config['AllowUploads'] = 'NO'; }
	        break;
		default:
			$eeSFL_Config['AllowUploads'] = 'NO'; // Show Nothing
	}
	
	if($eeSFL_Config['AllowUploads'] != 'NO' AND !$eeSFL_UploadFormRun) {
		if(!@$_POST['eeSFL_Upload'] AND !@$_POST['eeSFLS_Searching']) {
			$eeSFL_Nonce = wp_create_nonce('eeInclude');
			include($eeSFL_Env['pluginDir'] . '/includes/ee-upload-form.php');
			$eeSFL_UploadFormRun = TRUE;
		}
	}
	
	// Who Can View the List?
	switch ($eeSFL_Config['ShowList']) {
	    case 'YES':
	        break; // Show It
	    case 'USER':
	        // Show It If...
	        if( get_current_user_id() ) { break; } else { $eeSFL_Config['ShowList'] = 'NO'; }
	    case 'ADMIN':
	        // Show It If...
	        if(current_user_can('manage_options')) { break; } else { $eeSFL_Config['ShowList'] = 'NO'; }
	        break;
		default:
			$eeSFL_Config['ShowList'] = 'NO'; // Show Nothing
	}
	
	if($eeSFL_Config['ShowList'] != 'NO') {
		$eeSFL_Nonce = wp_create_nonce('eeInclude');
		include(WP_PLUGIN_DIR . '/' . $eeSFL->eePluginNameSlug . '/ee-list-display.php');
	}
	
	$eeOutput .= '</div>'; // Ends .eeSFL block
	
	$eeSFL_ListRun++;

	// Timer
	$eeSFL_Time = microtime(true) - $_SERVER["REQUEST_TIME_FLOAT"];
	$eeSFL_Log[] = 'Execution Time: ' . round($eeSFL_Time,3);
	
	// Logging
	if($eeSFL_DevMode) {
		// Add POST or GET to the beginning of the log
		if(@$_REQUEST) { array_unshift($eeSFL_Log, $_REQUEST); }
		$eeOutput .= '<pre id="eeSFL_DevMode">Log File ' . print_r($eeSFL_Log, TRUE) . '</pre>';
	}
	
	// Give it back
	unset($eeSFL_Files);
	unset($eeSFL_Env);
	unset($eeSFL_Config);
	unset($eeSFL_Log);
	
	return $eeOutput; // Output the page
}
add_shortcode( 'eeSFL', 'eeSFL_Shortcode' );



// Load Front-side <head>
function eeSFL_Enqueue() {
	
	// Register the style like this for a theme:
    wp_register_style( 'ee-simple-file-list-css', plugin_dir_url(__FILE__) . 'css/eeStyles.css', '', eeSFL_Cache_Version);
	wp_enqueue_style('ee-simple-file-list-css');
	
	// Javascript
	$deps = array('jquery'); // Requires jQuery
	
	$protocol = isset( $_SERVER['HTTPS'] ) ? 'https://' : 'http://';
	$params = array(
		'ajaxurl' => admin_url( 'admin-ajax.php', $protocol )
	);
	
	// Register Scripts
	wp_register_script( 'ee-simple-file-list-js-head', plugin_dir_url(__FILE__) . 'js/ee-head.js' );
	// wp_register_script( 'ee-simple-file-list-js-foot', plugin_dir_url(__FILE__) . 'js/ee-footer.js' );
	wp_register_script( 'ee-simple-file-list-js-uploader', plugin_dir_url(__FILE__) . 'js/ee-uploader.js' );
	
	// Enqueue
	wp_enqueue_script('ee-simple-file-list-js-head', plugin_dir_url(__FILE__) . 'js/ee-head.js', $deps, eeSFL_Cache_Version, FALSE); // Head
	wp_enqueue_script('ee-simple-file-list-js-foot', plugin_dir_url(__FILE__) . 'js/ee-footer.js',$deps, eeSFL_Cache_Version, TRUE); // Footer
	wp_enqueue_script('ee-simple-file-list-js-uploader', plugin_dir_url(__FILE__) . 'js/ee-uploader.js',$deps, eeSFL_Cache_Version, TRUE);
	
	// Pass variables
	wp_localize_script( 'ee-simple-file-list-js-foot', 'eesfl_vars', $params ); // Footer

}
add_action( 'wp_enqueue_scripts', 'eeSFL_Enqueue' );



// Admin <head>
function eeSFL_AdminHead($eeHook) {

	$deps = array('jquery');
	
	$protocol = isset( $_SERVER['HTTPS'] ) ? 'https://' : 'http://';
	$params = array(
		'ajaxurl' => admin_url( 'admin-ajax.php', $protocol )
	);
	
	// wp_die($eeHook);
    
    $eeHooks = array(
    	'toplevel_page_ee-simple-file-list', // toplevel_page_ee-simple-file-list
    	// 'simple-file-list_page_ee-simple-file-list',
    	// 'simple-file-list_page_ee-simple-file-list-settings',
    	'file-list_page_ee-simple-file-list-access'
    );
    
    if(in_array($eeHook, $eeHooks)) {
        
        // CSS
        wp_enqueue_style( 'ee-simple-file-list-css-front', plugins_url('css/eeStyles.css', __FILE__), '', eeSFL_Cache_Version );
        wp_enqueue_style( 'ee-simple-file-list-css-back', plugins_url('css/eeStyles-Back.css', __FILE__), '', eeSFL_Cache_Version );
        
        // Javascript
        wp_enqueue_script('ee-simple-file-list-js-head', plugin_dir_url(__FILE__) . 'js/ee-head.js', $deps, eeSFL_Cache_Version, FALSE);
		wp_enqueue_script('ee-simple-file-list-js-back', plugin_dir_url(__FILE__) . 'js/ee-back.js', $deps, eeSFL_Cache_Version, FALSE);
        wp_enqueue_script('ee-simple-file-list-js-foot', plugin_dir_url(__FILE__) . 'js/ee-footer.js', $deps, eeSFL_Cache_Version, TRUE);
        wp_enqueue_script('ee-simple-file-list-js-uploader', plugin_dir_url(__FILE__) . 'js/ee-uploader.js',$deps, eeSFL_Cache_Version, TRUE);
		
		wp_localize_script('ee-simple-file-list-js-head', 'eeSFL_JS', array( 'pluginsUrl' => plugins_url() ) ); // Needs expanding for alert boxes
		
		// Pass variables
		wp_localize_script( 'ee-simple-file-list-js-foot', 'eesfl_vars', $params ); // Footer
    }  
}
add_action('admin_enqueue_scripts', 'eeSFL_AdminHead');






// Ajax Handler
// Function name must be the same as the action name to work on front side ?
function simplefilelist_upload_job() {

	$eeResult = eeSFL_FileUploader();

	echo $eeResult;

	wp_die();

}	
add_action( 'wp_ajax_simplefilelist_upload_job', 'simplefilelist_upload_job' );


function simplefilelist_edit_job() {

	$eeResult = eeSFL_FileEditor();

	echo $eeResult;

	wp_die();

}	
add_action( 'wp_ajax_simplefilelist_edit_job', 'simplefilelist_edit_job' );



// File Upload Engine
function eeSFL_FileUploader() {
	
	// return 'TEST';
	
	global $eeSFL, $eeSFL_Config, $eeSFL_Log;
	
	// The FILE object
	if(empty($_FILES)) { 
		return 'Missing File Input';
	}
	
	if( !is_admin() ) { // Front-side protections
	
		// Who should be uploading?
		switch ($eeSFL_Config['AllowUploads']) {
		    case 'YES':
		        break; // Allow it, even if it's dangerous.
		    case 'USER':
		        // Allow it if logged in at all
		        if( get_current_user_id() ) { break; } else { return 'ERROR 97'; }
		    case 'ADMIN':
		        // Allow it if admin only.
		        if(current_user_can('manage_options')) { break; } else { return 'ERROR 97'; }
		        break;
			default: // Don't allow at all
				return 'ERROR 97';
		}
	}
	
	// The List ID
	if(@$_POST['eeSFL_ID']) { $eeSFL_ID = filter_var($_POST['eeSFL_ID'], FILTER_VALIDATE_INT); } else { $eeSFL_ID = FALSE; }
	if(!$eeSFL_ID) { return "Missing ID"; }
	
	// The Upload Destination - Relative to WP home dir
	if(@$_POST['eeSFL_FileUploadDir']) {
		
		$eeSFL_FileUploadDir = sanitize_text_field( urldecode($_POST['eeSFL_FileUploadDir']) );
		
		if(!$eeSFL_FileUploadDir) { return('Bad Upload Folder'); }
			
	} else { 
		return 'No Upload Folder Given';
	}
	
	// Check size
	$eeSFL_FileSize = filter_var($_FILES['file']['size'], FILTER_VALIDATE_INT);
	$eeSFL_UploadMaxFileSize = $eeSFL_Config['UploadMaxFileSize']*1024*1024; // Convert MB to B
	
	if($eeSFL_FileSize > $eeSFL_UploadMaxFileSize) {
		return "File size is too large.";
	}
	
	// Go...
	if($eeSFL_FileUploadDir AND is_dir(ABSPATH . $eeSFL_FileUploadDir)) {
			
		if(wp_verify_nonce(@$_POST['ee-simple-file-list-upload'], 'ee-simple-file-list-upload')) {
			
			// Temp file
			$eeTempFile = $_FILES['file']['tmp_name'];
			
			// Clean up messy names
			$eeSFL_FileName = eeSFL_SanitizeFileName($_FILES['file']['name']);
			
			eeSFL_DetectUpwardTraversal($eeSFL_FileUploadDir . $eeSFL_FileName); // Die if foolishness
			
			$eeSFL_PathParts = pathinfo($eeSFL_FileName);
			$eeSFL_FileNameAlone = $eeSFL_PathParts['filename'];
			$eeSFL_Extension = strtolower($eeSFL_PathParts['extension']); // We need to do this here and in eeSFL_ProcessUpload()
			
			// Format Check
			$eeSFL_FileFormatsArray = array_map('trim', explode(',', $eeSFL_Config['FileFormats']));
			
			if(!in_array($eeSFL_Extension, $eeSFL_FileFormatsArray) OR in_array($eeSFL_Extension, $eeSFL->eeForbiddenTypes)) {
				return 'File type not allowed: (' . $eeSFL_Extension . ')';	
			}
			
			// Assemble full name
			$eeSFL_TargetFile = $eeSFL_FileUploadDir . $eeSFL_FileNameAlone . '.' . $eeSFL_Extension;
			
			// Check if it already exists
			$eeSFL_TargetFile = eeSFL_CheckForDuplicateFile($eeSFL_TargetFile);
			
			$eeTarget = ABSPATH . $eeSFL_TargetFile;
			
			// Save the file
			if( move_uploaded_file($eeTempFile, $eeTarget) ) {
				
				if(!is_file($eeTarget)) {
					return 'Error - File System Error.'; // No good.
				} else {
					return 'SUCCESS';
				}
				 
			} else {
				return 'Cannot save the uploaded file: ' . $eeSFL_TargetFile;
			}
		
		} else {
			
			return 'ERROR 98';
		}
		
	} else {
		return 'Upload Path Not Found: ' . $eeSFL_FileUploadDir;
	}
}



// File Editor Engine
function eeSFL_FileEditor() {
	
	global $eeSFL, $eeSFLF, $eeSFL_Log;
	$eeFileName = '';
	$eeListFolder = '';
	$eeFileAction = '';
	
	// WP Security
	if( !check_ajax_referer( 'eeSFL_ActionNonce', 'eeSecurity' ) ) {
		return 'ERROR 98';	
	}
	
	// Check if we should be doing this
	if(is_admin() OR $eeSFL_Config['AllowFrontManage'] == 'YES') {
		
		// The List ID
		if(@$_POST['eeSFL_ID']) { $eeSFL_ID = filter_var($_POST['eeSFL_ID'], FILTER_VALIDATE_INT); } else { return "Missing ID"; }
		
		// The Action
		if(@$_POST['eeFileAction']) { 
			$eeFileAction = sanitize_text_field($_POST['eeFileAction']); 
		}
		if(!$eeFileAction) { 
			return "Missing the Action";
		}
				
		// The File Name
		if(@$_POST['eeFileName']) { 
			$eeFileName = sanitize_text_field($_POST['eeFileName']); 
		}
		if(!$eeFileName) { 
			return "Missing the Current File Name";
		}
		
		// Are we in a Folder?
		if(@$_POST['eeListFolder']) { 
			$eeListFolder = sanitize_text_field( urldecode( $_POST['eeListFolder'] )); 
		}
		if(!$eeListFolder OR $eeListFolder == '/') {
			$eeListFolder = '';
		}
		
		// Get the correct file list config if not main list
		$eeSFL_Config = $eeSFL->eeSFL_Config($eeSFL_ID);
		
		// Renaming
		if( strpos($eeFileAction, 'Rename') === 0 ) {
		
			// If Renaming
			if( strpos($eeFileAction, '|') ) {
				$eeArray = explode('|', $eeFileAction);
				$eeFileAction = $eeArray[0];
				$eeNewFileName = urldecode( $eeArray[1] );
				$eeNewFileName = eeSFL_SanitizeFileName($eeNewFileName);
			}
			
			if($eeNewFileName) {
				
				if(strpos($eeFileName, '.') === FALSE) { // Folder
					$eeNewFileName = str_replace('.', '_', $eeNewFileName); // Prevent adding an extension
				} else {
					$eePathParts = pathinfo($eeFileName);
					$eeOldExtension = strtolower($eePathParts['extension']); // Prevent changing file extension
					$eePathParts = pathinfo($eeNewFileName);
					$eeNewExtension = strtolower($eePathParts['extension']);
					if($eeOldExtension != $eeNewExtension) { 
						return "Changing File Extensions is Not Allowed";
					}	
				}
				
				eeSFL_DetectUpwardTraversal($eeSFL_Config['FileListDir'] . $eeNewFileName); // Die if foolishness
				
				$eeFullPath = ABSPATH . $eeSFL_Config['FileListDir'] . $eeListFolder;
				$eeOldFilePath = $eeFullPath . $eeFileName;
				$eeNewFilePath = $eeFullPath . $eeNewFileName;
				
				if( !rename($eeOldFilePath, $eeNewFilePath) ) {
					
					return 'Could Not Rename ' . $eeFileName . ' to ' . $eeNewFileName;
				
				} else {
					
					if(!strpos($eeFileName, '.')) { // Folder, need the trailing slash
						$eeFileName .= '/';
						$eeNewFileName .= '/';
					}
	 	               
					$eeSFL->eeSFL_UpdateFileDetail($eeSFL_ID, $eeListFolder . $eeFileName, 'FilePath', $eeListFolder . $eeNewFileName);
	 	               
					delete_transient('eeSFL_FileList-' . $eeSFL_ID); // Trigger a re-scan to rebuild thumbnail
					
					return 'SUCCESS';
				}
			
			} else { 
				return "Missing the New File Name";
			}
			
		} elseif($eeFileAction == 'Delete') {
			
			eeSFL_DetectUpwardTraversal($eeSFL_Config['FileListDir'] . $eeListFolder . $eeFileName); // Die if foolishness
			
			$eeFilePath = ABSPATH . $eeSFL_Config['FileListDir'] . $eeListFolder . $eeFileName;
			
			if( strpos($eeFileName, '.') ) { // Gotta be a File - Looking for the dot rather than using is_file() for better speed
				
				if(unlink($eeFilePath)) {
					
					delete_transient('eeSFL_FileList-' . $eeSFL_ID);
					
					return 'SUCCESS';
					
				} else {
					return __('File Delete Failed', 'ee-simple-file-list') . ':' . $eeListFolder . $eeFileName;
				}
			
			} else {
				
				// Delete Folder
				if($eeSFLF) {
					if( !$eeSFLF->eeSFLF_DeleteFolder($eeFilePath) ) {
						return __('Folder Delete Failed', 'ee-simple-file-list') . ':' . $eeListFolder . $eeFileName;
					} else {
						
						delete_transient('eeSFL_FileList-' . $eeSFL_ID); // Trigger a re-scan
						
						return 'SUCCESS';
					}
				}
			}
		
		} elseif($eeFileAction == 'UpdateDesc') {
			
			// The Description
			if(filter_var(@$_POST['eeFileID'], FILTER_VALIDATE_INT) !== FALSE) { // Might be a zero
				$eeFileID = $_POST['eeFileID'];
			} else { 
				$eeFileID = 0;
			}
			
			// The Description
			if(@$_POST['eeFileDesc']) { 
				$eeFileDesc = sanitize_text_field($_POST['eeFileDesc']); 
			} else { 
				$eeFileDesc = '';
			}
			
			if(!strpos($eeFileName, '.')) { // Folder, need the trailing slash
				$eeFileName .= '/';
			}
			
			// return $eeSFL_ID . ' - ' . $eeListFolder . $eeFileName . ' - ' . 'FileDescription' . ' - ' . $eeFileDesc;
			
			$eeSFL->eeSFL_UpdateFileDetail($eeSFL_ID, $eeListFolder . $eeFileName, 'FileDescription', $eeFileDesc);
		
			return 'SUCCESS';
			
		} else {
			
			return; // Nothing to do	
		}
	}
	
	// We should not be doing this
}








// Add Action Links to the Plugins Page
function eeSFL_ActionPluginLinks( $links ) {
	
	$eeLinks = array(
		'<a href="' . admin_url( 'admin.php?page=ee-simple-file-list' ) . '">' . __('Admin List', 'ee-simple-file-list') . '</a>',
		'<a href="' . admin_url( 'admin.php?page=ee-simple-file-list&tab=settings' ) . '">' . __('Settings', 'ee-simple-file-list') . '</a>'
	);
	return array_merge( $links, $eeLinks );
}
add_filter( 'plugin_action_links_' . plugin_basename(__FILE__), 'eeSFL_ActionPluginLinks' );



// Admin Pages
function eeSFL_AdminMenu() {
	
	global $eeSFL, $eeSFL_ID, $eeSFL_Config, $eeSFL_Env, $eeSFL_Log;
	
	// Only include when accessing the plugin admin pages
	if(@$_GET['page'] == $eeSFL->eePluginSlug) {
		
		$eeOutput = '<!-- Simple File List Admin -->';
		$eeSFL_Log['Admin'][] = 'Admin Menu Loading ...';
		
		$eeSFL_Nonce = wp_create_nonce('eeInclude'); // Security
		include_once(WP_PLUGIN_DIR . '/' . $eeSFL->eePluginNameSlug . '/ee-admin-page.php'); // Admin's List Management Page
	}
	
	// Admin Menu Visability
	switch ($eeSFL_Config['AdminRole']) {
	    case 1:
	        $eeCapability = 'read';
	        break;
	    case 2:
	        $eeCapability = 'edit_posts';
	        break;
	    case 3:
	        $eeCapability = 'publish_posts';
	        break;
	    case 4:
	        $eeCapability = 'edit_others_pages';
	        break;
	    case 5:
	        $eeCapability = 'activate_plugins';
	        break;
		default:
			$eeCapability = 'edit_posts';
	}
	
	// The Admin Menu
	add_menu_page(
		__($eeSFL->eePluginName, $eeSFL->eePluginSlug), // Page Title
		__($eeSFL->eePluginMenuTitle, $eeSFL->eePluginSlug), // Menu Title
		$eeCapability, // User status reguired to see the menu
		$eeSFL->eePluginSlug, // Slug
		'eeSFL_ManageLists', // Function that displays the menu page
		'dashicons-index-card' // Icon used
	);
	
}
add_action( 'admin_menu', 'eeSFL_AdminMenu' );



// Plugin Version Check
// We only run the update function if there has been a change in the database revision.
function eeSFL_VersionCheck() { 
		
	global $eeSFL_Log;
	
	$eeSFL_Log[] = 'Checking DB Version...';
	
	$eeSFL_DB_VersionInstalled = get_option('eeSFL-DB-Version'); // We store the DB version in the DB, okay?
	
	if($eeSFL_DB_VersionInstalled < eeSFL_DB_Version OR !get_option('eeSFL-Settings') ) {
		
		eeSFL_UpdateThisPlugin(); // Run the DB update process
	}
}



// Perform DB Update
function eeSFL_UpdateThisPlugin() {
	
	global $eeSFL, $eeSFL_Env, $eeSFL_Log;
	
	$eeSFL_DB_Version = get_option('eeSFL-DB-Version');
	
	if($eeSFL_DB_Version) {
		
		if( version_compare( $eeSFL_DB_Version, '4.2', '<') ) { 
			
			delete_transient('eeSFL_FileList-1'); // Force a re-scan because now we're storing a sorted file array.
			
			update_option('eeSFL-DB-Version', '4.2');
			
			return;
		
		} else {
	
			return; // Nothing more yet
		}
	}
	
	// New install or update from an old version
	
	$eeNewInstall = FALSE;
	
	$eeConfigDefault = $eeSFL->DefaultListSettings[1]; // Get our default config
	
	// Look for previous versions
	$eeSFL_V2 = get_option('eeSFL');
	$eeSFL_V3 = get_option('eeSFL-1-ShowList');
	
	// Upgrade Simple File List ?
	if($eeSFL_V3) { // Updating from 3.x
		
		$eeSFL_Log['Updating'][] = 'Version 3.x Detected';
		
		// Get Existing Settings
		$eeSFL_ShowList = get_option('eeSFL-1-ShowList');
		$eeSFL_ShowFileThumb = get_option('eeSFL-1-ShowFileThumb');
		$eeSFL_ShowFileDate = get_option('eeSFL-1-ShowFileDate');
		$eeSFL_ShowFileOwner = get_option('eeSFL-1-ShowFileOwner');
		$eeSFL_ShowFileSize = get_option('eeSFL-1-ShowFileSize');
		$eeSFL_SortBy = get_option('eeSFL-1-SortBy');
		$eeSFL_SortOrder = get_option('eeSFL-1-SortOrder');
		$eeSFL_ShowFileActions = get_option('eeSFL-1-ShowFileActions');
		$eeSFL_ShowHeader = get_option('eeSFL-1-ShowListHeader');
		$eeSFL_ShowFileThumb = get_option('eeSFL-1-ShowFileThumb');
		$eeSFL_AllowFrontDelete = get_option('eeSFL-1-AllowFrontDelete');
		
		// Uploading
		$eeSFL_FileListDir = get_option('eeSFL-1-UploadDir'); // Now FileListDir
		$eeSFL_AllowUploads = get_option('eeSFL-1-AllowUploads');
		$eeSFL_FileFormats = get_option('eeSFL-1-FileFormats');
		$eeSFL_UploadLimit = get_option('eeSFL-1-UploadLimit');
		$eeSFL_UploadMaxFileSize = get_option('eeSFL-1-UploadMaxFileSize');
		$eeSFL_GetUploaderInfo = get_option('eeSFL-1-GetUploaderInfo');
		// $eeSFL_TrackFileOwner = get_option('eeSFL-1-TrackFileOwner');
	
	
	} elseif( $eeSFL_V2 ) { // Updating from 1.x or 2.x
		
		// SFL Version 1
		// eeAllowList=Yes|eeAllowUploads=Yes|ee_upload_max_filesize=64|eeFormats=jpg,jpeg,png,pdf,zip|eeAdminTo=name@email.com
		
		// SFL Version 2 added...
		// eeFileOwner=No|eeFileListDir=wp-content/uploads/simple-file-list|eeSortList=Name|eeSortOrder=|eeShowForm=Yes
		
		// Get the existing settings, so we can convert them.
		$eeSettings = explode('|', $eeSFL_V2);
		
		// Version 1 settings
		$eeSetting = @explode('=', $eeSettings[0]); // Show the File List
		if($eeSetting[1] != 'Yes') { $eeSFL_ShowList = 'NO'; }
		
		$eeSetting = @explode('=', $eeSettings[1]); // AllowUploads
		if($eeSetting[1] != 'Yes') { $eeSFL_AllowUploads = 'NO'; }
			else { $eeSFL_AllowUploads = 'YES'; }
		
		$eeSetting = @explode('=', $eeSettings[2]); // Upload Max File size
		if($eeSetting[1]) { $eeSFL_UploadMaxFileSize = $eeSetting[1]; } else { $eeSFL_UploadMaxFileSize = 8; }
		
		$eeSetting = @explode('=', $eeSettings[3]); // Formats
		if($eeSetting[1]) { $eeSFL_FileFormats = $eeSetting[1]; }
		
		$eeSetting = @explode('=', $eeSettings[4]); // TO Email
		if($eeSetting[1]) { $eeSFL_Notify = $eeSetting[1]; }
		
		
		if(count($eeSettings) > 5) { // Version 2 Additions
			
			$eeSFL_Log['Updating'][] = 'Version 2.x Detected';
			
			$eeSetting = @explode('=', $eeSettings[5]); // Track File Owner
			if(@$eeSetting[1] != 'Yes') { $eeSFL_TrackFileOwner = 'NO'; }
			
			$eeSetting = @explode('=', $eeSettings[6]); // Upload Dir
			if(@$eeSetting[1]) { $eeSFL_FileListDir = $eeSetting[1]; }
			
			$eeSetting = @explode('=', $eeSettings[7]); // Sort List By...
			if(@$eeSetting[1]) { $eeSFL_SortBy = $eeSetting[1]; }
			
			$eeSetting = @explode('=', $eeSettings[8]); // Sort order
			if(@$eeSetting[1]) { $eeSFL_SortOrder = $eeSetting[1]; }
			
			$eeSetting = @explode('=', $eeSettings[9]); // Show Uploader Info Form
			if(@$eeSetting[1] == 'Yes') { $eeSFL_GetUploaderInfo = 'YES'; } else { $eeSFL_GetUploaderInfo = 'NO'; }
	
		} else {
			
			$eeSFL_Log['Updating'][] = 'Version 1.x Detected';
		}	
	
	} else {
		
		$eeSFL_Log['Updating'][] = 'New Installation';
		$eeNewInstall = TRUE; 
	}
	
	
	// Name Changes
	if(@$eeSFL_AllowFrontDelete == 'YES') { $eeSFL_AllowFrontManage = 'YES'; } else { $eeSFL_AllowFrontManage = FALSE; }



	// Notification Changes
	$eeNotifyOld = get_option('eeSFL-Notify'); // Old way, no ID
	$eeNotifyNew = get_option('eeSFL-1-Notify'); // New way, with ID
	
	if( strpos($eeNotifyOld, '@') ) {
		$eeSFL_NotifyTo = $eeNotifyOld;
		$eeSFL_Notify = 'YES';
		// delete_option('eeSFL-Notify'); // Out with the old.
	} elseif( strpos($eeNotifyNew, '@') ) {
		$eeSFL_NotifyTo = $eeNotifyNew; // In with the new.
		$eeSFL_Notify = 'YES';
	} elseif(@$eeSFL_Notify) { // V2
		$eeSFL_NotifyTo = $eeSFL_Notify;
		$eeSFL_Notify = 'YES';
	} else {
		$eeSFL_Notify = $eeConfigDefault['Notify'];
		$eeSFL_NotifyTo = get_option('admin_email');
	}
	
	
	// Assign Default if No Value
	if(!@$eeSFL_ShowList) { $eeSFL_ShowList = $eeConfigDefault['ShowList']; }
	if(!@$eeSFL_ShowFileThumb) { $eeSFL_ShowFileThumb = $eeConfigDefault['ShowFileThumb']; }
	if(!@$eeSFL_ShowFileDate) { $eeSFL_ShowFileDate = $eeConfigDefault['ShowFileDate']; }
	if(!@$eeSFL_ShowFileSize) { $eeSFL_ShowFileSize = $eeConfigDefault['ShowFileSize']; }
	if(!@$eeSFL_ShowFileActions) { $eeSFL_ShowFileActions = $eeConfigDefault['ShowFileActions']; }
	if(!@$eeSFL_SortBy) { $eeSFL_SortBy = $eeConfigDefault['SortBy']; }
	if(!@$eeSFL_SortOrder) { $eeSFL_SortOrder = $eeConfigDefault['SortOrder']; }
	if(!@$eeSFL_ShowHeader) { $eeSFL_ShowHeader = $eeConfigDefault['ShowHeader']; }
	if(!@$eeSFL_AllowFrontManage) { $eeSFL_AllowFrontManage = $eeConfigDefault['AllowFrontManage']; }
	if(!@$eeSFL_FileFormats) { $eeSFL_FileFormats = $eeConfigDefault['FileFormats']; }
	if(!@$eeSFL_UploadLimit) { $eeSFL_UploadLimit = $eeConfigDefault['UploadLimit']; }
	if(!@$eeSFL_UploadMaxFileSize) { $eeSFL_UploadMaxFileSize = $eeSFL_Env['the_max_upload_size']; }
	if(!@$eeSFL_GetUploaderInfo) { $eeSFL_GetUploaderInfo = $eeConfigDefault['GetUploaderInfo']; }
	if(!@$eeSFL_AllowUploads) { $eeSFL_AllowUploads = $eeConfigDefault['AllowUploads']; }
	
	
	// The File List Directory ----------------
	
	// Create the File List Dir if Needed
	if(!@$eeSFL_FileListDir) {
		
		$eeSFL_FileListDir = $eeSFL_Env['FileListDefaultDir'];
		eeSFL_FileListDirCheck( $eeSFL_FileListDir ); // Create the File List Folder
	
	} else {
	
		// Check if FileListDir has a trailing slash...
		$eeLastChar = substr($eeSFL_FileListDir, -1);
		if($eeLastChar != '/') {  $eeSFL_FileListDir .= '/'; } // Add the slash, required for 3.1 +
		
		// Check if FileListDir has a leading slash
		if($eeSFL_FileListDir[0] == '/') {  $eeSFL_FileListDir = substr($eeSFL_FileListDir, 1); } // Omit the slash, required for 4 +
		
		eeSFL_FileListDirCheck( $eeSFL_FileListDir ); // Check the File List Folder
	}
	
	// Add First File
	if($eeNewInstall) { // Copy the instructions PDF to the file list folder
		
		$eeCopyFrom = dirname(__FILE__) . '/Simple-File-List.pdf';
		$eeCopyTo = ABSPATH . '/' . $eeSFL_FileListDir . 'Simple-File-List.pdf';
		copy($eeCopyFrom, $eeCopyTo);
	}
	
	
	// Create Settings Array --------------
	
	$eeSettings = array( // See $DefaultListSettings within ee-class.php for definitions
		
		'1' => array(
			
			'ListTitle' => 'Simple File List', // NEW in SFL 4
			'FileListDir' => $eeSFL_FileListDir,
			'ExpireTime' => $eeConfigDefault['ExpireTime'], // NEW in SFL 4
			'ShowList' => $eeSFL_ShowList,
			'AdminRole' => $eeConfigDefault['AdminRole'], // NEW in SFL 4
			'ShowFileThumb' => $eeSFL_ShowFileThumb,
			'ShowFileDate' => $eeSFL_ShowFileDate,
			'ShowFileSize' => $eeSFL_ShowFileSize,
			'SortBy' => $eeSFL_SortBy,
			'SortOrder' => $eeSFL_SortOrder,
			
			'AllowUploads' => $eeSFL_AllowUploads,
			'UploadLimit' => $eeSFL_UploadLimit,
			'UploadMaxFileSize' => $eeSFL_UploadMaxFileSize,
			'FileFormats' => $eeSFL_FileFormats,
			
			'PreserveSpaces' => $eeConfigDefault['PreserveSpaces'],
			'ShowFileDescription' => $eeConfigDefault['ShowFileDescription'], // NEW in SFL 4
			'ShowFileActions' => $eeSFL_ShowFileActions,
			'ShowFileExtension' => $eeConfigDefault['ShowFileExtension'], // NEW in SFL 4
			'ShowHeader' => $eeSFL_ShowHeader,
			'ShowUploadLimits' => $eeConfigDefault['ShowUploadLimits'], // NEW in SFL 4
			'GetUploaderInfo' => $eeSFL_GetUploaderInfo,
			'ShowSubmitterInfo' => $eeConfigDefault['ShowSubmitterInfo'], // NEW in SFL 4
			'AllowFrontSend' => $eeConfigDefault['AllowFrontSend'], // NEW in SFL 4
			'AllowFrontManage' => $eeSFL_AllowFrontManage,
			
			'Notify' => $eeSFL_Notify,
			'NotifyTo' => $eeSFL_NotifyTo, // NEW in SFL 4
			'NotifyCc' => '', // NEW in SFL 4
			'NotifyBcc' => '', // NEW in SFL 4
			'NotifyFrom' => get_option('admin_email'), // NEW in SFL 4
			'NotifyFromName' => '', // NEW in SFL 4
			'NotifySubject' => '', // NEW in SFL 4
			'NotifyMessage' => $eeSFL->eeNotifyMessageDefault // NEW in SFL 4
		)
	);
	
	$eeSFL_Log['Updating'][] = $eeSettings;
	
	// Update the Option		
	update_option('eeSFL-Settings', $eeSettings); // NEW in SFL 4
	
	// Create first file list array
	$eeFilesArray = array();
	update_option('eeSFL-FileList-1', $eeFilesArray); // NEW in SFL 4	
	
	// Update the DB Version
	update_option('eeSFL-DB-Version', eeSFL_DB_Version);
	
	// TO DO in SFL 4.1 -- Delete ALL old options
	delete_option('eeSFL-Legacy'); // Don't need this anymore

	$eeSFL_Log['Updating'][] = 'Plugin Updated to database version: ' . eeSFL_DB_Version;
	
	// Write the log file to the Database
	$eeSFL->eeSFL_WriteLogData($eeSFL_Log);

}


// Plugin Activation ==========================================================
function eeSFL_Activate() {
	
	// TO DO - Check extension versions - Fail unless they are updated first.
	
	return TRUE; // All done, nothing to do here.	
}
register_activation_hook( __FILE__, 'eeSFL_Activate' );

?>