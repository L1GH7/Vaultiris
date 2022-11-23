<?php // Simple File List Script: ee-list-display.php | Author: Mitchell Bennis | support@simplefilelist.com
	
if ( ! defined( 'ABSPATH' ) ) exit; // Exit if accessed directly
if ( ! wp_verify_nonce( $eeSFL_Nonce, 'eeInclude' ) ) exit('ERROR 98'); // Exit if nonce fails

$eeSFL_Files = FALSE;
$eeSFL_ListClass = 'eeSFL'; // The basic list's CSS class. Extensions might change this.
$eeClass = ''; // Meaning, CSS class
$eeSFL_AllowFrontManage = 'NO'; // Front-side freedom
$eeSendFilesArray = array(); // Used for the Add More Files in the Send Files overlay
$eeSFL_SendFile_AddFileArray = array(); // We use this for the Send File function
$eeUploadedFiles = FALSE;
$eeDateFormat = get_option('date_format');
$eeSFL_ActionNonce = wp_create_nonce('eeSFL_ActionNonce'); // Security for Ajax
global $eeSFL_ListRun;

$eeSFL_Log['File List'][] = 'Loaded: ee-list-display';
$eeSFL_Log['File List'][] = 'Listing Files in: ' . $eeSFL_Config['FileListDir'];

// Required extension variable definitions, even if not installed.
$eeSFLF_FolderOptionsDisplay = FALSE;
$eeSFLF_FolderDepth = 1;
global $eeSFLF_ListFolder;

// echo '<pre>'; print_r($_POST); echo '</pre>';

// Get the File List
if( (@$_GET['eeSFL_Scan'] === 'true' AND $eeAdmin) OR @$eeSFL_Config['ExpireTime'] === 0) { // Only admins can force a rescan
	
	$eeSFL_Files = $eeSFL->eeSFL_UpdateFileListArray($eeSFL_ID);
	
} else {
	
	$eeSFL_Log['File List'][] = 'Checking List Freshness...';
	$eeCheckFreshness = get_transient('eeSFL_FileList-' . $eeSFL_ID); // Get the File List Transient
	
	if($eeCheckFreshness == 'Good') { // Get the list
		
		$eeSFL_Log['File List'][] = 'Fresh :-)';
		
		$eeSFL_Files = get_option('eeSFL-FileList-' . $eeSFL_ID); // Get the File List
		
	} else { // Update the list
		
		$eeSFL_Log['File List'][] = 'Expired :-(';
		
		$eeSFL_Files = $eeSFL->eeSFL_UpdateFileListArray($eeSFL_ID); // Stale, so Re-Scan
	}
	
	// If not found, rescan
	if(!$eeSFL_Files AND $eeAdmin) { 
		$eeSFL_Log['errors'] = __('No File List Found. Please Re-Scan', 'ee-simple-file-list');
	}
}

// echo '<pre>'; print_r($eeSFL_Config); echo '</pre>';

// echo '<pre>'; print_r($eeSFL_Files); echo '</pre>'; exit;

// Shortcode sorting att used
if($eeForceSort) { // Sorting is usually only done when the disk is scanned
	$eeSFL_Files = $eeSFL->eeSFL_SortFiles($eeSFL_Files, $eeSFL_Config['SortBy'], $eeSFL_Config['SortOrder']);
}

// Save for later
$eeSFL_FileTotalCount = 0;
$eeSFL_FolderTotalCount = 0;
$eeSFL_ItemTotalCount = $eeSFL_FileTotalCount + $eeSFL_FolderTotalCount;

// Total in whole list
if( !empty($eeSFL_Files) ) {
	foreach( $eeSFL_Files as $eeKey => $eeFileArray ) {
		if($eeFileArray['FileExt'] == 'folder') { $eeSFL_FolderTotalCount++; } 
			else { $eeSFL_FileTotalCount++; }
	}
} else {
	$eeSFL_Files = array();
}


$eeSFL_ListNumber = $eeSFL_ListRun; // Legacy 04/20

// Extension Check
if($eeSFLF) {
	$eeSFLF_Nonce = wp_create_nonce('eeSFLF_Include'); // Security
	include(WP_PLUGIN_DIR . '/ee-simple-file-list-folders/eeSFLF_folderListSetup.php');
} else {
	foreach( $eeSFL_Files as $eeKey => $eeFileArray) { // Send Files List
		if(!strpos($eeFileArray['FilePath'], '/')) { // Omit folders for now
			$eeSendFilesArray[] = $eeFileArray;
		}
	}
}

// Only show files just uploaded
if(@$eeSFL_Env['UploadedFiles']) {
	
	$eeUploadedFiles = array();
	
	foreach( $eeSFL_Files as $eeKey => $eeFileArray) {
		
		if( in_array($eeFileArray['FilePath'], $eeSFL_Env['UploadedFiles']) ) {
			$eeUploadedFiles[] = $eeFileArray;
		}
		
		if(count($eeUploadedFiles)) {
			$eeSFL_Files = $eeUploadedFiles;
		} else {
			$eeSFL_Files = FALSE;
		}
	}
	
} else {
	$eeSFL_Env['UploadedFiles'] = '';
}


// Getting Ready...

// Extension Check
if($eeSFLS) {
	
	$eeAllFilesSorted = $eeSFL_Files; // Legacy
	
	// $eeSFLS_TotalItemsCount = count($eeSFL_Files); // Before search
	$eeSFLS_Nonce = wp_create_nonce('eeSFLS_Include'); // Security
	include(WP_PLUGIN_DIR . '/ee-simple-file-list-search/includes/ee-search-processor.php'); // Run the Search Processor
	$eeSFLS_Nonce = wp_create_nonce('eeSFLS_Include'); // Security
	include(WP_PLUGIN_DIR . '/ee-simple-file-list-search/includes/ee-pagination-processing.php'); // Run Pagination Processing
}


// User Messaging	
if(@$eeSFL_Log['messages'] AND $eeSFL_ListRun == 1) { 
	$eeOutput .=  eeSFL_ResultsDisplay($eeSFL_Log['messages'], 'notice-success');
	$eeSFL_Log['messages'] = array(); // Clear
}	
if(@$eeSFL_Log['errors']) { 
	$eeOutput .=  eeSFL_ResultsDisplay($eeSFL_Log['errors'], 'notice-error');
	$eeSFL_Log['errors'] = array(); // Clear
}


// DISPLAY ===================================================

$eeOutput .= '

<!-- File List -->
<span class="eeSFL_Hide" id="eeSFL_ID">' . $eeSFL_ID . '</span>
<span class="eeSFL_Hide" id="eeSFL_ActionNonce">' . $eeSFL_ActionNonce . '</span>
<script>
	var eeSFL_ListID = ' . $eeSFL_ID . ';
	var eeSFL_PluginURL = "' . $eeSFL_Env['pluginURL'] . '";
	var eeSFL_FileListDir = "' . $eeSFL_Config['FileListDir'] . '";
	var eeSFL_ListFolder = "' . $eeSFLF_ListFolder . '/' . '";
</script>
';

$eeURL = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";

// Back-side Buttons
if($eeAdmin) {
	
	$eeOutput .= '<div id="eeAdminActions">
	
	<p class="eeRight">
	
		<a href="#" class="button eeButton" id="eeSFL_UploadFilesButton">' . __('Upload Files', 'ee-simple-file-list') . '</a>';
	 
						
	if($eeSFL_Config['ExpireTime']) {
		$eeOutput .= '<a href="#" class="button eeButton" id="eeSFL_ReScanButton">' . __('Re-Scan Files', 'ee-simple-file-list') . '</a>';
	}
	
	// If No Extension
	if(!@defined('eeSFLF_Version')) { $eeOutput .= '
		
		<a href="/wp-admin/admin.php?page=ee-simple-file-list&tab=pro&eeListID=' . $eeSFL_ID . '" class="button eeButton" >' . __('Create Folder', 'ee-simple-file-list') . '</a>'; // Add Folder Support
	}
	
	if(!@defined('eeSFLS_Version') AND $eeSFL_FileTotalCount > 11) { $eeOutput .= '
		
		<a href="/wp-admin/admin.php?page=ee-simple-file-list&tab=pro&eeListID=' . $eeSFL_ID . '" class="button eeButton" >' . __('Search Files', 'ee-simple-file-list') . '</a>'; // Add Search & Pagination, if 12+
	}
	
	$eeOutput .= '</p>';
	
	// If showing just-uploaded files
	if($eeSFL_Uploaded) { 
		
		$eeOutput .= '
		
		<p class="eeSFL_ListMeta"><a href="' . eeSFL_AppendProperUrlOp($eeURL) . 'eeListID=' . $eeSFL_ID . '" class="button eeButton" id="eeSFL_BacktoFilesButton">&larr; ' . __('Back to the Files', 'ee-simple-file-list') . '</a></p>';
		
		$eeSendFilesArray = $eeSFL_Files; // Restrict to just what was uploaded
	
	} else {
		
		// Calc Date Last Changed
		$eeArray = array();
		if(count($eeSFL_Files)) {
			foreach( $eeSFL_Files as $eeKey => $eeFileArray) { $eeArray[] = $eeFileArray['FileDateChanged']; }
			rsort($eeArray); // Most recent at the top	
		} else {
			$eeArray[] = date('Y-m-d H:i:s'); // today
		}
		
		$eeOutput .= '
		
		<p class="eeSFL_ListMeta">' . __('Files', 'ee-simple-file-list') . ': ' . $eeSFL_FileTotalCount . ' | '  . 
		__('Folders', 'ee-simple-file-list') . ': ' . $eeSFL_FolderTotalCount . ' | ' . 
		__('Sorted by', 'ee-simple-file-list') . ': ' . ucwords($eeSFL_Config['SortBy']) . ' (' . $eeSFL_Config['SortOrder'] . ')<br />
		' . __('Last Changed', 'ee-simple-file-list') . ': ' . date_i18n( $eeDateFormat, strtotime( $eeArray[0] ) );
		
		$eeOutput .= '</p>';
	}
	
	$eeOutput .= '
	
		<br class="eeClearFix" />
	
	</div>';

} elseif($eeSFL_Uploaded AND $eeSFL_ListRun == 1) {
	
	$eeOutput .= '<p class="eeSFL_ListMeta"><a href="' . $eeURL . '" class="button eeButton" id="eeSFL_BacktoFilesButton">&larr; ' . 
		__('Back to the Files', 'ee-simple-file-list') . '</a></p>';
		
	$eeSendFilesArray = $eeSFL_Files; // Restrict to just what was uploaded
}


// Extension Checks	
if($eeSFLF) {
	$eeOutput .= $eeSFLF_FunctionBar;
	$eeOutput .= '<span class="eeSFL_Hide" id="eeSFLF_ListFolder">' . $eeSFLF_ListFolder . '</span>
	<span class="eeSFL_Hide" id="eeSFLF_MoveNonce">' . $eeSFLF_MoveNonce . '</span>';
}

if($eeSFLS AND !$eeSFL_Uploaded) {
	$eeSFLS_Nonce = wp_create_nonce('eeSFLS_Include'); // Security
	include(WP_PLUGIN_DIR . '/ee-simple-file-list-search/includes/ee-search-form.php');
}

// $eeSFL_Files = array_values($eeSFL_Files);

// echo '<pre>'; print_r($eeSFL_Files); echo '</pre>';	


// TABLE HEAD ==================================================================================================

if( strlen( @$eeSFL_Files[0]['FilePath'] ) >= 1 ) {
	
	// if(!$eeSFL_Files[0]['FilePath']) { return; }
	
	$eeRowID = '0'; // Assign an ID number to each row
	
	$eeOutput .= '<table class="eeFiles">';
	
	if($eeSFL_Config['ShowHeader'] == 'YES' OR $eeAdmin) { $eeOutput .= '<thead><tr>';
							
		if($eeSFL_Config['ShowFileThumb'] == 'YES') { 
			
			$eeOutput .= '<th class="eeSFL_Thumbnail">';
			
			if(@$eeSFL_Config['LabelThumb']) { $eeOutput .= stripslashes($eeSFL_Config['LabelThumb']); } 
				else { $eeOutput .= __('Thumb', 'ee-simple-file-list'); }
			
			$eeOutput .= '</th>';
		}
		
		
		$eeOutput .= '<th class="eeSFL_Sortable eeSFL_FileName">';
			
		if(@$eeSFL_Config['LabelName']) { $eeOutput .= stripslashes($eeSFL_Config['LabelName']); } 
			else { $eeOutput .= __('Name', 'ee-simple-file-list'); }
		
		$eeOutput .= '</th>';
		
		
		if($eeSFL_Config['ShowFileSize'] == 'YES') { 
			
			$eeOutput .= '<th class="eeSFL_Sortable eeSFL_FileSize">';
			
			if(@$eeSFL_Config['LabelSize']) { $eeOutput .= stripslashes($eeSFL_Config['LabelSize']); } 
				else { $eeOutput .= __('Size', 'ee-simple-file-list'); }
			
			$eeOutput .= '</th>';
		}
		
		
		if($eeSFL_Config['ShowFileDate'] == 'YES') { 
			
			$eeOutput .= '<th class="eeSFL_Sortable eeSFL_FileDate">';
			
			if(@$eeSFL_Config['LabelDate']) { $eeOutput .= stripslashes($eeSFL_Config['LabelDate']); } 
				else { $eeOutput .= __('Date', 'ee-simple-file-list'); }
			
			$eeOutput .= '</th>';
		}

		
		$eeOutput .= '</tr></thead>';
	}						
	
	$eeOutput .= '<tbody>';
					
	$eeFileCount = 0; // Reset
	$eeListPosition = FALSE; // eeSFLS
	
	$eeSFL_Log['File List'][] = 'Listing Files...';
	
	if(is_array($eeSFL_Files)) {
							
		// Loop through array
		foreach($eeSFL_Files as $eeFileKey => $eeFileArray) { // <<<---------------------------- BEGIN FILE LIST LOOP ----------------<<<
			
			$eeRowID = @$eeFileArray['FileID']; // Set in sorting method, but not sorted if only one file
			if(!$eeRowID) { $eeRowID = 0; } // Only one file, set ID to zero
			
			// Go
			if( is_array($eeFileArray)) {
	
				$eeFilePath = $eeFileArray['FilePath']; // Path relative to FileListDir
				$eeFileName = basename($eeFilePath); // Just the name
				$eeFileDate = date_i18n( $eeDateFormat, strtotime( $eeFileArray['FileDateChanged'] ) ); // The mod date, make nice per WP config
				$eeFileDateAdded = date_i18n( $eeDateFormat, strtotime( $eeFileArray['FileDateAdded'] ) );
				$eeFileSize = eeSFL_FormatFileSize($eeFileArray['FileSize']); // The file size made nice too
				
				// Extension Check
				if(!$eeListPosition AND $eeListPosition !== 0) { // eeSFLS
					$eeListPosition = $eeFileKey; // Get the first key
				}
				
				// Ready, set...
				$eeIsFile = FALSE;
				$eeIsFolder = FALSE;
				$eeFileURL = FALSE;
				$eeFileExt = FALSE;
				$eeFileThumbURL = FALSE;
				
				// Fix display if Folder Extension is turned off
				if(!$eeSFLF AND strpos($eeFileArray['FilePath'], '/')) {
					$eeFileName = FALSE; // Will not show
				}
				
				// Go
				if($eeFileName) {
				
					$eeFileCount++; // Bump the file count
					
					if( strpos($eeFilePath, '.') ) { // This is a file
						
						$eeIsFile = TRUE;
						
						$eeSFL_SendFile_AddFileArray[$eeFileCount] = $eeFilePath; // Used for Send -> Add Files
						
						$eeFileURL = $eeSFL_Config['FileListURL'] . $eeFileArray['FilePath']; // Clickable URL
						
						$eeFileURL = str_replace('://', '\\:', $eeFileURL); // Save this
						$eeFileURL = str_replace('//', '/', $eeFileURL); // Remove double slashes
						$eeFileURL = str_replace('\\:', '://', $eeFileURL); // Restore that
						$eeFileExt = $eeFileArray['FileExt']; // Get Extension
						
						// Extension Check
						if($eeSFLS AND $eeSFLF) { // Get List Folder from path to ensure the thumb displays in search result
							if(@$eeSFLS_FileSearchArray) {
								$eeFile = basename($eeFileArray['FilePath']);
								$eeSFLF_ListFolder = str_replace($eeFile, '', $eeFileArray['FilePath']);
							}
						}
					
					} elseif($eeSFLF) { // Extension Check
						
						$eeIsFolder = TRUE;
						$eeFileExt = 'folder';
						$eeFileURL = $eeSFLF->eeSFLF_GetFolderURL($eeFilePath, $eeSFLF_ShortcodeFolder); // Extension required
					
					} else {
						
						$eeFilePath = FALSE;
					}
					
					
					// Skip names hidden via shortcode
					if(@$eeSFL_HideName) {
						
						$eeArray = explode(',', $eeSFL_HideName);
						
						foreach( $eeArray as $eeKey => $eeValue ) {
							
							if( strtolower($eeFileName) ==  $eeValue . '.' . $eeFileExt ) { // Without extension
								continue(2); // Go to next file
							}
							
							if($eeValue == strtolower($eeFileName)) { // With extension, or folder name
								continue(2); // Go to next file
							}
						}
					}
					
					
					// Skip types hidden via shortcode
					if(@$eeSFL_HideType) {
						if(strpos($eeSFL_HideType, $eeFileExt) OR strpos($eeSFL_HideType, $eeFileExt) === 0 ) { 
							continue; // Go to next file
						}
					}
					
					
					// Start The List --------------------------------------------------------------
				
					$eeOutput .= '
					
					<tr id="eeSFL_RowID-' . $eeRowID . '">'; // Add an ID to use in javascript
					
					
					// Thumbnail
					if($eeSFL_Config['ShowFileThumb'] == 'YES') {
						
						// Create Thumbnail Path
						$eeIsImage = in_array($eeFileExt,  $eeSFL->eeDynamicImageThumbFormats);
						
						// Do we have FFmpeg ?
						if($eeSFL_Env['supported']) {
							$eeIsVideo = in_array($eeFileExt,  $eeSFL->eeDynamicVideoThumbFormats);
						} else {
							$eeIsVideo = FALSE; // Nope
						}
						
						// Check Type
						if($eeIsImage OR $eeIsVideo) { // Images use .jpg files
							
							$eePathParts = pathinfo($eeSFL_Config['FileListDir'] . $eeFilePath);
							$eeFileNameOnly = $eePathParts['filename'];
							
							$eeFileThumbURL = $eeSFL_Config['FileListURL'];
							if($eeSFLF_ListFolder) { $eeFileThumbURL .= $eeSFLF_ListFolder . '/'; } // Adjust to path if needed
							$eeFileThumbURL .= '.thumbnails/thumb_' . $eeFileNameOnly . '.jpg';
						
						} else { // Others use our own .svg files
							
							if( !in_array($eeFileExt, $eeSFL->eeDefaultThumbFormats) ) {
								$eeDefaultThumb = '!default.svg'; // What the heck is this?
							} else {
								$eeDefaultThumb = $eeFileExt . '.svg';
							}
							
							$eeFileThumbURL = $eeSFL_Env['wpPluginsURL'] . $eeSFL->eePluginNameSlug . '/images/thumbnails/' . $eeDefaultThumb;
						}
					
						$eeOutput .= '<td class="eeSFL_Thumbnail">';
						
						if($eeFileThumbURL) { $eeOutput .= '<a href="' . $eeFileURL .  '"';
							
							if($eeIsFolder) { $eeOutput .= ' class="eeFolderIcon"'; } else { $eeOutput .= ' target="_blank"'; }
								
							$eeOutput .= '><img src="' . $eeFileThumbURL . '" width="64" height="64" alt="Thumbnail for ' . $eeFileName . '" /></a>'; }
						
						$eeOutput .= '</td>';
					}
					
					
					
					
					
					
					// NAME
					$eeOutput .= '<td class="eeSFL_FileName">';
					
					if($eeFileURL) {
						
						$eeRealFileName = $eeFileName; // Save for editing
						
						$eeOutput .= '<span class="eeSFL_RealFileName eeSFL_Hide">' . $eeRealFileName . '</span>
						
						<p class="eeSFL_FileLink"><a class="eeSFL_FileName" href="' . $eeFileURL .  '"';
						if(!$eeIsFolder) {
							$eeOutput .= ' target="_blank"';
						}
						$eeOutput .= '>';
						
						
						// Extension Check
						if($eeSFLF) { // Show a small folder icon before the name if thumbs are not used.
							
							if($eeIsFolder AND $eeSFL_Config['ShowFileThumb'] != 'YES') {
								$eeOutput .= '<b class="eeSFL_FolderIconSmall">' . $eeSFLF_FolderIcon . '</b> '; // <<<--------- MOVE TO INCLUDE
							}
						
							// Path display for searching in folders
							if(@$_POST['eeSFLS_Searching']) {
								
								$eeArray = explode('/', $eeFilePath);
								if( count($eeArray) > 1 ) { 
									unset($eeArray[0]); // Remove this folder
								}
								$eeFileName = implode('/', $eeArray);
							}
						}
						
						// Strip the extension?
						if(!$eeAdmin AND $eeSFL_Config['ShowFileExtension'] == 'NO') {
							$eeSFL_PathParts = pathinfo($eeFileName);
							$eeFileName = $eeSFL_PathParts['filename'];
						}
						
						// Replace hyphens with spaces?
						if(!$eeAdmin AND $eeSFL_Config['PreserveSpaces'] == 'YES') {
							$eeFileName = eeSFL_PreserveSpaces($eeFileName); 
						}
						
						$eeOutput .= $eeFileName . '</a></p>';
						
						// Show File Description, or not.
						if(@$eeFileArray['FileDescription'] OR @$eeFileArray['SubmitterComments']) { // @$eeSFL_Config['showFileDesc'] != 'YES' OR 
							
							$eeClass = ''; // Show
							if(!@$eeFileArray['FileDescription']) {
								$eeFileArray['FileDescription'] = $eeFileArray['SubmitterComments']; // Show the submitter comment if no desc
							}
							
						} else {
							$eeClass = 'eeSFL_Hide';
						}
						
						
						// This is always here because for js access
						$eeOutput .= '<p class="eeSFL_FileDesc ' . $eeClass . '">';
						
						if($eeSFL_Config['ShowFileDescription'] == 'YES') {
							$eeOutput .= stripslashes(@$eeFileArray['FileDescription']);
						}
						 
						$eeOutput .= '</p>';
						
						
						// Submitter Info
						if(@$eeFileArray['SubmitterName']) {
								
							if($eeAdmin OR $eeSFL_Config['ShowSubmitterInfo'] == 'YES') {
								
								$eeOutput .= '<p class="eeSFL_FileSubmitter">
								
								' . __('Submitted by', 'ee-simple-file-list') . ': <a href="mailto:' . $eeFileArray['SubmitterEmail'] . '">' . $eeFileArray['SubmitterName'] . '</a></p>';
							}
						}
						
						// File Actions   ------------------------------------------------------------------------------------
						
						if($eeAdmin OR $eeSFL_Config['ShowFileActions'] == 'YES') { // Always show to Admin
							
							// Construct
							$eeFileActions = '
							
								<small class="eeSFL_ListFileActions">';
								
							if(in_array($eeFileExt, $eeSFL->eeOpenableFileFormats)) {
								$eeFileActions .= '<a class="eeSFL_FileOpen" href="' . $eeFileURL . '"';
								if(!$eeIsFolder) { $eeFileActions .= ' target="_blank"'; }
								$eeFileActions .= '>' . __('Open', 'ee-simple-file-list') . '</a> | ';
							}
							
							if($eeIsFile) {
								
								$eeFileActions .= '<a class="eeSFL_FileDownload" href="' . $eeFileURL . '" download="' . $eeFileName . '">' . __('Download', 'ee-simple-file-list') . '</a> | ';
								
								if($eeAdmin OR $eeSFL_Config['AllowFrontSend'] == 'YES') {
									$eeFileActions .= '<a href="" onclick="eeSFL_SendFile(' . $eeRowID . ')">' . __('Send', 'ee-simple-file-list') . '</a> | ';
								}
								
							}
														
							
							// Append Addition (admin or authorized) Actions
							if( ($eeAdmin OR $eeSFL_Config['AllowFrontManage'] == 'YES') AND $eeSFL_ListRun == 1) {
								
								$eeFileActions .= '<a href="" id="eeSFL_EditFile_' . $eeRowID . '" onclick="eeSFL_EditFile(' . $eeRowID . ')">' . __('Edit', 'ee-simple-file-list') . '</a> | ';
								
								if($eeSFLF AND $eeSFL_FolderTotalCount) { // Extension Check
									
									$eeSFLF_FolderOptionsDisplay = FALSE;
									$eeSFLF_FolderOptionsDisplay = $eeSFLF->eeSFLF_MoveToFolderDisplay($eeFileArray, $eeSFLF_ListFolder);
									
									$eeMoveLink = '<a id="eeSFLF_moveLink_' . $eeRowID . '" href="#" onclick="eeSFLF_MoveFileDisplay(' . $eeRowID . ')">' . __('Move', 'ee-simple-file-list') . '</a>';	
									
									if($eeIsFolder) {
										
										if($eeSFLF_ListFolder OR $eeSFL_FolderTotalCount > 1) { // Don't show in main folder if only one folder
											
											if($eeSFLF_FolderOptionsDisplay) { $eeFileActions .= $eeMoveLink; }
										}
										
									} else {
										$eeFileActions .= $eeMoveLink;
									}
								} 
								
								
							}
							
							// Extension Check
							if(!$eeSFLF AND $eeAdmin) {
								$eeFileActions .= '<a class="eeDimmedLink" href="/wp-admin/admin.php?page=ee-simple-file-list&tab=pro" >' . __('Move', 'ee-simple-file-list') . '</a>';
							}
							
							// Strip trailing pipe if needed
							if(substr($eeFileActions, -2) == '| ') {
								$eeFileActions = substr($eeFileActions, 0, -3);
							}
								
							$eeFileActions .= '</small>'; // Close action links
							
								
							
							// Expanding Inputs
							if($eeAdmin OR $eeSFL_Config['AllowFrontManage'] == 'YES') {	
								
								// Move-to-folder select box
								if($eeSFLF) {
									if( ($eeAdmin OR $eeSFL_Config['AllowFrontManage'] == 'YES') ) { $eeFileActions .= $eeSFLF_FolderOptionsDisplay; }
								}
								
								
								// Javascript-powered Drop-Down Box
								$eeFileActions .= '<div class="eeSFL_EditFileWrap" id="eeSFL_EditFileWrap_' . $eeRowID . '">
								
								<h4>' . __('Edit Details', 'ee-simple-file-list') . '</h4>';
								
								$eeFileActions .= '<p><label>' . __('File Name', 'ee-simple-file-list') . '
								<input required="required" type="text" class="eeNewFileName" name="eeNewFileName" value="' . $eeRealFileName . '" size="32" /></label></p>
									
								<p class="eeSFL_FileDesc_in"><label>' . __('Description', 'ee-simple-file-list') . '
									<span class="eeSFL_SavedDesc">' . @$eeFileArray['FileDescription'] . '</span>
									<input type="text" class="eeSFL_NewFileDesc" name="eeSFL_FileID_' . $eeFileKey . '" value="' . @$eeFileArray['FileDescription'] . '" size="32" /></label></p> 
									
								<p class="eeCenter">
									<a class="button" href="#" onclick="eeSFL_EditSave(' . $eeRowID . ')">' . __('Save', 'ee-simple-file-list') . '</a> 
									<a class="button" href="#" onclick="eeSFL_EditFile(' . $eeRowID . ')">' . __('Cancel', 'ee-simple-file-list') . '</a> 
									<a class="button eeDeleteButton" href="#" onclick="eeSFL_Delete(' . $eeRowID . ')">' . __('Delete', 'ee-simple-file-list') . '</a>
								</p>
								
								<p class="eeCenter"><small>' . __('Added', 'ee-simple-file-list') . ': ' . $eeFileDateAdded . ' — ';
								
								// Show Mod Date if different
								if($eeFileArray['FileDateAdded'] != $eeFileArray['FileDateChanged'] ) { $eeFileActions .= __('Modified', 'ee-simple-file-list') . ': ' . $eeFileDate . ' — '; }
								
								$eeFileActions .= __('Size', 'ee-simple-file-list') . ': ' . $eeFileSize . '</small></p>
									
								</div>';
							}
								
							$eeOutput .= $eeFileActions;
						}
					}
					
					$eeOutput .= '</td>';
					
					
					
					
					
					// File Size
					if($eeSFL_Config['ShowFileSize'] == 'YES') {
					
						$eeOutput .= '<td class="eeSFL_FileSize">';
						
						if($eeIsFile) {
							
							$eeOutput .= $eeFileSize;
							
						} else {
							
							if($eeSFLF) {
								if($eeSFL_Config['ShowFolderSize'] == 'YES') {
									$eeOutput .= eeSFL_GetFileSize($eeSFLF->eeSFLF_GetFolderSize($eeSFL_Config['FileListDir'] . $eeFilePath));
								} else {
									$eeOutput .= __('Folder', 'ee-simple-file-list');
								} 
							}
						}
						
						$eeOutput .= '</td>';
					}
					
					
					
					
					
					// File Modification Date
					if($eeSFL_Config['ShowFileDate'] == 'YES') {
						
						$eeOutput .= '<td class="eeSFL_FileDate">' . $eeFileDate . '</td>';
					}
					
					
					$eeOutput .= '</tr>';
				}
				
				$eeRowID++; // Bump the ID
			}
		} // END loop
	
	}
	
	
	$eeOutput .= '
	
	</tbody></table>
	
	<p class="eeSFL_Hide">
			<span id="eeSFL_FilesCount">' . $eeFileCount . '</span>
		</p>'; // This allows javascript to access the info
	
	$eeSFL_Env['FileLists'] = ''; // Remove to clean up display
	// $eeSFL_Log['Files'] = $eeSFL_Files;
	
	// Pagination Controls
	if($eeSFLS) {
		$eeSFLS_Nonce = wp_create_nonce('eeSFLS_Include'); // Security
		include(WP_PLUGIN_DIR . '/ee-simple-file-list-search/includes/ee-pagination-display.php');
	}
	
	if($eeAdmin OR ($eeSFL_ListRun == 1 AND $eeSFL_Config['AllowFrontSend']) ) {
		
		// Send Files Overlay - Hidden until the Send link is clicked
		$eeOutput .= '<div id="eeSFL_SendPop" class="eeActive">
		
			<article>
				
				<h2>' . __('Send Link', 'ee-simple-file-list') . '</h2>
				
				<p>' . __('Send an email with links to the files. Add more files if needed.', 'ee-simple-file-list') . '</p>
				
				<p id="eeSFL_SendTheseFilesList">' . __('Files to be sent:', 'ee-simple-file-list') . ' <em></em></p>
				
				<form id="eeSFL_SendFileForm" action="' . eeSFL_GetThisURL() . '" method="POST">
					
					<fieldset id="eeSFL_SendInfo">
							
						<label for="eeSFL_SendFrom">' . __('Your Address', 'ee-simple-file-list'). '</label>
							<input required type="email" name="eeSFL_SendFrom" value="" size="64" id="eeSFL_SendFrom" />
							
						<label for="eeSFL_SendTo">' . __('The TO Address', 'ee-simple-file-list'). '</label>
							<input required type="email" name="eeSFL_SendTo" value="" size="64" id="eeSFL_SendTo" />
							
						<label for="eeSFL_SendCc">' . __('The CC Address', 'ee-simple-file-list'). '</label>
							<input type="text" name="eeSFL_SendCc" value="" size="64" id="eeSFL_SendCc" />
						<div class="eeClearFix eeNote">' . __('Separate multiple addresses with a comma', 'ee-simple-file-list') . '</div>
							
						<label for="eeSFL_SendSubject">' . __('The Subject', 'ee-simple-file-list'). '</label>
							<input type="text" name="eeSFL_SendSubject" value="" size="64" id="eeSFL_SendSubject" />
							
						<label for="eeSFL_SendMessage">' . __('The Message', 'ee-simple-file-list'). '</label>
							<textarea name="eeSFL_SendMessage" id="eeSFL_SendMessage" cols="64" rows="5"></textarea>
							
						<br class="eeClearFix" />
					
						<p class="eeSFL_SendButtons">';
							
						$eeCount = @count($eeSendFilesArray);
						
						if($eeCount > 1) {
							$eeOutput .= '<button onclick="eeSFL_Send_AddMoreFiles();">' . __('Add Files', 'ee-simple-file-list') . '</button> ';
						}	
							
						$eeOutput .='<button onclick="eeSFL_Send_Cancel();">' . __('Cancel', 'ee-simple-file-list') . '</button>
							<input type="submit" name="eeSFL_Send" value="' . __('Send', 'ee-simple-file-list') . ' &rarr;" />
						</p>
					
					</fieldset>';
					
					if( $eeCount > 1) {
					
						$eeOutput .= '
						
						<fieldset id="eeSFL_SendMoreFiles">
					
						<h3>' . __('Add More Files', 'ee-simple-file-list') . '</h3>
						
						<table>
						 <tbody>';
						 
						foreach( $eeSendFilesArray as $eeKey => $eeFileArray) {
							
							$eeFileNameDisplay = $eeFileArray['FilePath'];
							
							if($eeSFLF_ListFolder) {
								if( strpos($eeFileArray['FilePath'], $eeSFLF_ListFolder) === 0 ) {
									$eeFileNameDisplay = str_replace($eeSFLF_ListFolder, '', $eeFileArray['FilePath']); // Remove this folder's name from the display
								}
							}
							
							if($eeFileArray['FileExt'] != 'folder') { // We can't send folders, yet.
							
								$eeOutput .= '
							
								<tr>
								    <td class="eeAlignRight eeSFL_AddFileID_' . $eeFileArray['FileID'] . '"><input type="checkbox" name="eeSFL_SendTheseFiles[]" value="' . urlencode($eeFileArray['FilePath']) . '"/></td>
								    <td>' . $eeFileNameDisplay . '</td>
								</tr>';
							}
						}		 
						 
						$eeOutput .= '</tbody>
							</table>
						
							<p class="eeSFL_SendButtons">
								<button onclick="eeSFL_Send_AddTheseFiles();">' . __('Add Files', 'ee-simple-file-list') . '</button>
									<button onclick="eeSFL_Send_AddMoreCancel();">' . __('Cancel', 'ee-simple-file-list') . '</button>
							</p>
						
						</fieldset>';
					
					}
					
				$eeOutput .= '
				
				</form>
			
			</article>
	
		</div>'; // End Send Overlay
	
	}
	
} elseif( !@$_POST['eeSFLS_Searching'] ) {
	
	$eeSFL_Log['File List'][] = 'There are no files here :-(';
	
	if($eeAdmin) {
		$eeOutput .= '<div id="eeSFL_noFiles"><p>&#8593; ' . __('Upload some files and they will appear here.', 'ee-simple-file-list') . '</p></div>';
	}
}

$eeSFL_Log['Environment'] = $eeSFL_Env;
$eeSFL_Log['Config'] = $eeSFL_Config;

?>