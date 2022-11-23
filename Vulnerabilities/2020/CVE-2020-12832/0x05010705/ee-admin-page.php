<?php // Simple File List Script: ee-admin-page.php | Author: Mitchell Bennis | support@simplefilelist.com
	
if ( ! defined( 'ABSPATH' ) ) exit; // Exit if accessed directly
if ( ! wp_verify_nonce( $eeSFL_Nonce, 'eeInclude' )) exit('ERROR 98'); // Exit if nonce fails

$eeSFL_Log['Admin'][] = 'Loaded: ee-admin-page';

// Admin-Side Display
function eeSFL_ManageLists() {
	
	global $eeSFL, $eeSFL_Log, $eeSFL_DevMode, $eeSFL_ID, $eeSFL_Config, $eeSFL_Env, $eeSFL_ListRun;
	global $eeSFLF, $eeSFLS; // Extensions
	
	$eeSFL_Files = FALSE;
	$eeForceSort = FALSE; // Only used in shortcode
	
	$eeAdmin = is_admin(); // Will be TRUE here
	
	// Process the Config Array
	if(!is_array($eeSFL_Config) OR !is_array($eeSFL_Env)) {
		$eeSFL_Log['errors'] = 'No SFL Configuration';
		return FALSE;
	}
	
	// Begin Output
	$eeOutput = '
	
	<div class="eeSFL_Admin wrap">
	<div class="eeSFL" id="eeSFL">'; 
	
	if($eeSFL_DevMode) { $eeOutput .= '<p class="eeAlert">' . __('DEVELOPMENT MODE ON', 'ee-simple-file-list') . '</p>'; }
	
	// Get the new tab's query string value. We will only use values to display tabs that we are expecting.
	if( isset( $_GET[ 'tab' ] ) ) { $active_tab = sanitize_text_field($_GET[ 'tab' ]); } else { $active_tab = 'file_list'; }
	
	$eeOutput .= '<h2 class="nav-tab-wrapper">';
	
	// Main Tabs -------
	
	// File List
	$eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=file_list&eeListID=' . $eeSFL_ID . '" class="nav-tab tabList ';  
	if($active_tab == 'file_list') {$eeOutput .= '  eeActiveTab ';}    
    $active_tab == 'file_list' ? 'nav-tab-active' : '';
    $eeOutput .= $active_tab . '">' . __('File List', 'ee-simple-file-list') . '</a>';
    
    // Author
    $eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=author&eeListID=' . $eeSFL_ID . '" class="nav-tab tabSupport ';   
	if($active_tab == 'author') {$eeOutput .= '  eeActiveTab '; }  
    $active_tab == 'author' ? 'nav-tab-active' : ''; 
    $eeOutput .= $active_tab . '">' . __('Author', 'ee-simple-file-list') . '</a>';
    
	// The Help / Email Form Page
    $eeOutput .= '<a href="mailto:support@simplefilelist.com" class="nav-tab tabSupport">' . __('Get Help', 'ee-simple-file-list') . '</a>';
    
    // Settings
    $eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=settings&eeListID=' . $eeSFL_ID . '" class="nav-tab tabSettings ';   
	if($active_tab == 'settings') {$eeOutput .= '  eeActiveTab '; }  
    $active_tab == 'settings' ? 'nav-tab-active' : ''; 
    $eeOutput .= $active_tab . '">' . __('Settings', 'ee-simple-file-list') . '</a>';
    
    // Shortcode Builder
    $eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=shortcode&eeListID=' . $eeSFL_ID . '" class="nav-tab tabSettings ';  
	if($active_tab == 'shortcode') {$eeOutput .= '  eeActiveTab '; }   
    $active_tab == 'support' ? 'nav-tab-active' : ''; 
    $eeOutput .= $active_tab . '">' . __('Create Shortcode', 'ee-simple-file-list') . '</a>';
    
    // Get Extensions
    if(!$eeSFLF OR !$eeSFLS) {
	    $eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=pro&eeListID=' . $eeSFL_ID . '" class="nav-tab tabSupport ';   
		if($active_tab == 'pro') {$eeOutput .= '  eeActiveTab '; }  
	    $active_tab == 'pro' ? 'nav-tab-active' : ''; 
	    $eeOutput .= $active_tab . '">' . __('Get Pro', 'ee-simple-file-list') . '</a>';
    }
    
    $eeOutput .= '</h2>'; // END Main Tabs   
    
    
    // Tab Content =============================================================
    
	if($active_tab == 'file_list') {
	
		$eeOutput .= '<div id="uploadFilesDiv">';
		
		// Upload Check
		$eeSFL_Nonce = wp_create_nonce('eeInclude'); // Security
		include($eeSFL_Env['pluginDir'] . '/includes/ee-upload-check.php');
		
		$eeSFL_Nonce = wp_create_nonce('eeInclude'); // Security
		include($eeSFL_Env['pluginDir'] . 'includes/ee-upload-form.php'); // The Uploader
		
		$eeOutput .= '</div>';
		
		$eeSFL_Nonce = wp_create_nonce('eeInclude'); // Security
		include($eeSFL_Env['pluginDir'] . 'ee-list-display.php'); // The File List		
	
	} elseif($active_tab == 'settings') {
		
		// Sub Tabs
		if( isset( $_GET[ 'subtab' ] ) ) { $active_subtab = sanitize_text_field($_GET['subtab']); } else { $active_subtab = 'list_settings'; }
	    	
    	$eeOutput .= '<p class="eeTitle">' . __('File List Settings', 'ee-simple-file-list') . '</p>
    	
    	<h2 class="nav-tab-wrapper">';
		
		// List Settings
		$eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=settings&subtab=list_settings&eeListID=' . $eeSFL_ID . '" class="nav-tab ';  
		if($active_subtab == 'list_settings') {$eeOutput .= '  eeActiveTab ';}    
	    $active_subtab == 'list_settings' ? 'nav-tab-active' : '';    
	    $eeOutput .= $active_subtab . '">' . __('File List Settings', 'ee-simple-file-list') . '</a>';
	    
	    // Uploader Settings
		$eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=settings&subtab=uploader_settings&eeListID=' . $eeSFL_ID . '" class="nav-tab ';  
		if($active_subtab == 'uploader_settings') {$eeOutput .= '  eeActiveTab ';}    
	    $active_subtab == 'uploader_settings' ? 'nav-tab-active' : '';    
	    $eeOutput .= $active_subtab . '">' . __('File Upload Settings', 'ee-simple-file-list') . '</a>';
	    
	    // Display Settings
		$eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=settings&subtab=display_settings&eeListID=' . $eeSFL_ID . '" class="nav-tab ';  
		if($active_subtab == 'display_settings') {$eeOutput .= '  eeActiveTab ';}    
	    $active_subtab == 'display_settings' ? 'nav-tab-active' : '';    
	    $eeOutput .= $active_subtab . '">' . __('Display Settings', 'ee-simple-file-list') . '</a>';
	    
	    // Notifications Settings
		$eeOutput .= '<a href="?page=' . $eeSFL->eePluginSlug . '&tab=settings&subtab=email_settings&eeListID=' . $eeSFL_ID . '" class="nav-tab ';  
		if($active_subtab == 'email_settings') {$eeOutput .= '  eeActiveTab ';}    
	    $active_subtab == 'email_settings' ? 'nav-tab-active' : '';    
	    $eeOutput .= $active_subtab . '">' . __('Notification Settings', 'ee-simple-file-list') . '</a>';
	    
	    $eeOutput .= '</h2>'; // END Subtabs
	    
		if($active_subtab == 'uploader_settings') {
			
			$eeSFL_Nonce = wp_create_nonce('eeInclude');
			include($eeSFL_Env['pluginDir'] . 'includes/ee-upload-settings.php'); // The Uploader Settings
		
		} elseif($active_subtab == 'display_settings') {
			
			$eeSFL_Nonce = wp_create_nonce('eeInclude');
			include($eeSFL_Env['pluginDir'] . 'includes/ee-list-display-settings.php'); // The List Display Settings
		
		} elseif($active_subtab == 'email_settings') {
			
			$eeSFL_Nonce = wp_create_nonce('eeInclude');
			include($eeSFL_Env['pluginDir'] . 'includes/ee-email-settings.php'); // The Notifications Settings
		
		} else {
			
			$eeSFL_Nonce = wp_create_nonce('eeInclude');
			include($eeSFL_Env['pluginDir'] . 'includes/ee-list-settings.php'); // The File List Settings
		}
		
	} elseif($active_tab == 'shortcode') { // Shortcode Builder Tab Display...
			
		// Get the instructions page
		$eeSFL_Nonce = wp_create_nonce('eeInclude');
		include($eeSFL_Env['pluginDir'] . 'includes/ee-shortcode-builder.php');
	
	
	} elseif($active_tab == 'pro') { // Instructions Tab Display...
			
		// Get the sales page
		$eeSFL_Nonce = wp_create_nonce('eeInclude');
		include($eeSFL_Env['pluginDir'] . 'includes/ee-get-pro.php');
	
	
	} elseif($active_tab == 'help') { // Email Support Tab Display...
		
		$eePlugin = $eeSFL->eePluginName;
			
		// Get the support page
		$eeSFL_Nonce = wp_create_nonce('eeInclude');
		include($eeSFL_Env['pluginDir'] . 'support/ee-get-help.php');
	
	
	} else { // Author
					
		// Get the support page
		$eeSFL_Nonce = wp_create_nonce('eeInclude');
		include($eeSFL_Env['pluginDir'] . 'includes/ee-plugin-author.php');
		
	} // END Tab Content
	
	
	$eeSFL_Nonce = wp_create_nonce('eeInclude');
	include('includes/ee-admin-footer.php');
	
	$eeOutput .= '</div></div>';
	
	// Logging
	$eeSFL_Log['Admin'][] = 'Displaying Tab: ' . $active_tab . ' ' . @$active_subtab;
	$eeSFL_Log['Config'][] = $eeSFL_Config;
	// $eeSFL_Log['Files'][] = $eeSFL_Files;
	// if(@$_POST) { array_unshift($eeSFL_Log, $_POST); } // Add POST to the beginning of the log for
	
	// Timer
	$eeSFL_Time = microtime(true) - $_SERVER["REQUEST_TIME_FLOAT"];
	$eeSFL_Log[] = 'Execution Time: ' . round($eeSFL_Time,3); 
	
	// Visual Logging
	if($eeSFL_DevMode) { // Display the log at the bottom of the page.
		$eeOutput .= '<pre id="eeSFL_DevMode">Log File ' . print_r( array_filter($eeSFL_Log), TRUE) . '</pre>';
	}

	// Output the page
	echo $eeOutput;
}

?>