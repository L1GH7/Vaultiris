=== Simple File List ===
Contributors: eemitch
Donate link: http://simplefilelist.com/donations/simple-file-list-project/
Tags: file sharing, file list, file uploader, upload files, share files, exchange files, host files, sort files, dropbox, ftp
Requires at least: 4.0
Tested up to: 5.4.1
Stable tag: 4.2.7
License: GPLv2 or later
License URI: http://www.gnu.org/licenses/gpl-2.0.html

Simple File List gives your WordPress website a list of your files which allows your users to open and download them.

== Description ==

Simple File List is a free plugin that gives your WordPress website a list of your files allowing your users to open and download them. Users can also upload files if you choose.

Simple File List is also an alternative to using FTP or Dropbox for larger files. There's no need to deal with Dropbox or FTP usernames and passwords anymore! Everything is on your Wordpress website.

* Manage your files and the list settings from the Admin List.
* Complete settings for: list display and performance, file uploading restrictions, upload notifications and additional functionality options. 
* Both the Front-side List and Uploader can be shown to users based on their role; Everyone, Only Logged-in User, Only Admins or Nobody.
* Optionally collect the uploader's name, email and description of the file(s). This can then be shown in the file list.
* Files can also be assigned descriptions, which can be added from the Admin list or user uploads. Descriptions can be shown or hidden.
* Use the Send option to send someone an email with a link to your file.
* Optionally allow your front-side users full control over renaming, moving, sending, deleting and editing descriptions.


= This Plugin is Great For =

* Posting official documents.
* Sharing files within an organization.
* Sharing files with business clients or a community.
* Enabling distance learning by allowing schools to share class materials with students. 
* When you need a list of archived files, such as videos, PDF files, or music files.
* When you need a simple front-side uploader so people can send you files.
* Exchanging files when the sizes get too large for email attachments.


= List Features =

* Limit access to only Admins or logged-in users, or hide the list and only show the uploader.
* Add and manage your files from the Admin List.
* Show columns for file date, size and a thumbnail for images and videos.
* Add descriptions to files and optionally show them in your list.
* Sort file by name, date or file size ... ascending or descending.
* Thumbnail images are generated automatically for images and videos (ffMpeg required).
* Send emails with links to your files to others.
* Files are kept separate from the media library


= Uploader Features =

* Simple reliable uploader, works on mobile devices too.
* Drag and drop zone, plus upload progress bar
* Allow uploading to only Admins or logged-in users, or turn it off completely.
* Limit the types of files users can upload.
* Limit number of files uploaded per submission.
* Limit the maximum upload file size.
* Get an email notice each time a file is uploaded.
* Option to gather the uploader's name, email and comments.


= Shortcode Attributes =

* Over-ride the main list's settings on a per page/post/widget basis using shortcode attributes.
* Shortcode Builder allows you to create custom file list shortcodes.
** Choose to create a new draft post/page with the shortcode in place.
* List Visibility: showlist="YES / USER / ADMIN / NO"
* Uploader Visibility: allowuploads="YES / USER / ADMIN / NO"
* File Thumbnail Column: showthumb="YES / NO"
* File Size Column: showsize="YES / NO"
* File Date Column: showdate="YES / NO"
* File List Appearance: showheader="YES / NO"
* File Actions (Open | Download): showactions="YES / NO"
* List Sort By: sortby="Name, Date, Size, or Random"
* List Sort Order: sortorder="Descending or Ascending"
* Hide Files: hidename="this-file.docx, that-folder"
* Hide File Types: hidetype="psd, zip, folder"


= Internationalized =

* de_DE - German (Germany)
* es_ES - Spanish (Spain)
* es_MX - Spanish (Mexico)
* fr_CA - French (Canada)
* fr_FR - French (France)
* fr_BE - French (Belgium)
* it_IT - Italian (Italy)
* ja_JP - Japanese (Japan)
* nl_NL - Dutch (Netherlands)
* pt_BR - Portuguese (Brazil)
* pt_PT - Portuguese (Portugal)
* sv_SE - Swedish (Sweden)


= Plus =

* Simple lightweight design, easy to style and customize.
* Committed support from the developer, including internal support contact form.

= PRO Version (Optional) =

* Create unlimited levels of sub-folders.
* Use a shortcode attribute to display specific folders.
** [eeSFL showfolder="folderA/folderB"]
* Display different folders in different places on your site.
* You can even show several different folders on the same page and within widgets.
* Front-side users cannot navigate above the folder you specify.
* Breadcrumb navigation indicates where you are.
* Easily move files and folders as needed.
* Rename folders and add descriptions, which can be shown in the file list.
* Quickly delete any folder, along with all contents.
* Choose to sort folders first or sort along with the files.
* Optionally display folder sizes.
* Optionally define a custom file list directory.

= PRO ADD-ON - Search & Pagination (Optional) =

* Adds searching and pagination functionality.
* Designed to make very large file lists more manageable.
* Adds a search bar above the file list.
* Search by file name and/or date, if this column is displayed.
* Searches within sub-folders. (But not above the current folder)
* Pagination breaks up large file lists into smaller pages.
* Define the number of files per page in the settings.
* Show or hide the search bar and/or pagination in the settings.
* Updating to newer versions is just like other Wordpress plugins.
* Shortcode attributes to control search visibility and pagination functionality.
** [eeSFL search="YES/NO" paged="YES/NO" filecount="25"]
* Use a shortcode to place a search form anywhere on your website.
** [eeSFLS permalink='file-list-url']


== Installation ==

1. To install, simply upload the plugin zip file to your Wordpress website and activate it. Configure the simple settings in the new menu that will appear in your sidebar. Explanations of each feature accompany the inputs.
2. To add the plugin to your website, simply add this shortcode: [eeSFL]
3. Customize the shortcode using attributes listed above.


== Frequently Asked Questions ==

= Q: Who is Simple File List for? =

A: This script is for media companies, print houses, educational institutions, music sites, etc… Anyone who exchanges files with clients and customers, or within a group setting.

= Q: Are the file uploaded to the Media Library? =

A: No, files are uploaded to a special folder inside your general WordPress uploads folder, or anywhere you specify.

= Q: Can people who upload overwrite existing files? =

A: No, files cannot be overwritten. If a file is uploaded having the same name as one already present, a series number is appended to the name ( filename_(2).ext ) You can use the renaming functionality to change this.

= Q: Can I place different lists in different places? =

A: Yes, with the optional Folder Support extension you can put different file lists on different posts, pages and widgets.

= Q: Are the files in the list searchable by Google and other search engines? =

A: Only if you place a list on the front-side of your website which is viewable to the general public. If you choose USER, ADMIN or NO, in the List Settings these files will not be indexed by search engines.

= Q: Can I customize the appearance of the list and uploader? =

A: Yes, the CSS is easily over-ridden, making it easy for anyone with CSS knowledge to customize the page design. Check the Instructions page within the plugin's admin panel for more info.

= Q: Can I create custom behavior after an upload completes? =

A: Yes, you can hook into the actions "eeSFL_UploadCompleted" and "eeSFL_UploadCompletedAdmin" and do pretty much anything you want upon upload completion.

= Q: What is the maximum upload file size? =

A: This is a setting that you choose in the file configuration. The initial default is 8 MB. The absolute maximum size will depend on your hosting setup. which is automatically detected.

= Q: What if I have trouble or need assistance? Will you help? =

A: Yes! I enjoy helping people. Please contact me with any issues using the built in support form.

= Q: Why did you develop this plugin? =

A: I got tired of the difficulties of getting files back and forth between myself and my non-technical clients once they become too large for email. Training these people to use FTP or Dropbox was a challenge. I wanted something simple that I could use on my own website, so I created a simple index.php page that solved my problem. I later realized that others could benefit from this functionality, so I decided to port it to my favorite website platform; Wordpress. I also hoped that the donations would pay for a large home and a private jet, but that has not happened yet :-(  Regardless, I still enjoy giving to the community and helping others.



== Upgrade Notice ==

* 4.2.7 - Various bug fixes and Improvements


== Screenshots ==

1. Front-side display, both upload and list activated.
2. Back-side display, both upload and list are always activated.
3. Back-side settings page.


== Changelog ==

= 4.2.7 =
* Speed and server load improvements.
* Fixed a couple of bugs with sorting.
* Fixed a bug where descriptions were not sticking to folders or files after renaming the file.
* Improved the French translations, added French-Belgium translations.
* Various other improvements


= 4.2.6 =
* Various security improvements
* Code cleanup and Ajax Improvements

= 4.2.3 =
* Disallowed changing the file type when renaming, as this presented a security risk for untrusted users.
* Add sorting shortcodes [eeSFL sortby="Name, Date, Size, or Random" sortorder="Descending or Ascending"]
* Fixed where clicking the Open link for a folder was opening in a new tab.
* Improved the upload info form's validation. Name and email are now required if getting uploader info.
* Fixed an issue where both renaming a file and adding/editing the description caused the description to be applied to the wrong file.


= 4.2.2 =
* Fixed a bug where some had a bad experience upon updating.

= 4.2.1 =
* File Access Manager (coming soon) integration
* Improved flexibility when using multiple shortcodes per page.
* Now auto-detecting a logged-in user and auto-populating the name and email fields on the upload form.
* Added option to allow uploaded files to over-write existing files, rather than numbering new files with the same name.
* No longer hiding upload settings if the uploader is set to not display.
* Fixed a site crash after adding a very very large image file.
* CSS Improvements
* Various bug fixes and improvements

= 4.1.3 =
* Added shortcodes for Search & Pagination customization
** [eeSFL paged="(YES/NO)" filecount="25" search="(YES/NO)"]

= 4.1.2 =
* Fixed missing thumbnails when FFmpeg is installed, but fails to read the source video format.

= 4.1.1 =

* Added new admin-side action hook: eeSFL_UploadCompletedAdmin
* Video thumbnails were not being created via FFmpeg
* Thumbnail images were not following proper path
* Addressed Error 99 issue on Windows installations
* Various bug fixes and code improvements

= 4.1.0 =

* Improved the uploader with a drag-and-drop zone, plus a file upload progress bar.
* Added Send File overlay to optionally allow sharing links to a file, or files, via email.
* Added file descriptions with option to show on front-side or not.
* Added saving of upload info form inputs which can optionally be displayed as the file description on the front-side.
* Created a new after-upload view which shows only the file(s) uploaded.
* Added ability to customize the file lists heading labels.
* Added ability to show or hide the file extension.
* Added ability to preserve spaces in file names, which are normally replaced with hyphens.
* Expanded configuration options for the upload notification emails (from, from name, to, cc, bcc, subject, and body text)
* Added ability to restrict access to the plugin’s list and upload settings by Wordpress user role
* New Shortcode: hidename="this-file.docx, that-folder"  Hide specific file names in the list. Accepts a comma list of file names.
* New Shortcode: hidetype="psd, zip, folder"  Hide specific file types in the list. Accepts a comma list of file extensions.
* Implemented a database-based file lists to improve performance and allow for meta data tracking.
* Added UploadDir status transient, instead of checking each time.
* Added option to set list re-scan interval every 0 to 24 hours. Zero will work like old versions, accessing the file system on each load.
* Moved settings to a single array in wp_options, instead of each separately, reducing code and database hits significantly.
* Rewrote the single-level file directory listing method, using scandir() instead of the old opendir() method.
* Improved styling, with much improved table display on small screens
* Changed Rename and Delete links to use new AJAX routines. Removed the delete checkbox form and created File Edit accordion below each file.
* Expanded settings, separating general list settings from list display options.

= 3.2.17 =

* Bug fix where email notices were failing to send to multiple addresses.

= 3.2.16 =

* File URL bug fix

= 3.2.15 =

* Improved Search & Pagination integration

= 3.2.14 =

* Translation updates and a minor change related to extension updating

= 3.2.13 =

* Improvements and new features for Folder Support

= 3.2.12 =

* Fixed the admin-side ugly arrow issue when file names create broken paths.
* Changed "Add Folder Support" to "Add Features".
* Now fully supports the new Search & Pagination extension.

= 3.2.11 =

* Further improved thumbnail load times.
* Revamped the file download process.

= 3.2.10 =

* Front-side-delete bug fixes.
* Improved thumbnail loading speed.
* Unseen fixes, improvements and progress.

= 3.2.9 =

* Minor bug fix

= 3.2.8 =

* Further security improvements to the file downloader.

= 3.2.7 =

* Improved file upload process.
* Added support for optional search & pagination (coming soon)
* Patched a security issue with the file downloader engine.

= 3.2.6 =

* Bug fix to allow files with uppercase extensions to upload properly.

= 3.2.5 =

* Fixed broken link within the email notice if the file name had spaces.
* Fixed a security issue with the file downloader and deletion.

= 3.2.4 =

* Addressed a bug causing extension updating failure
* Improved process logging for better support

= 3.2.3 =

* NEW Shortcode Builder: Dynamically create custom file list shortcode. Folder support ready.
* Now you can automatically create a new draft post/page with your shortcode.
* Fixed a bug where the default thumbnail SVG was not appearing for unaccounted-for file types
* Minor Admin UI changes

= 3.2.2 = 

* Fixed an issue where jQuery was not being loaded on some themes.
* Fixed an issue where File Thumbs were being turned off upon plugin update
* Fixed a few other minor bugs

= 3.2.1 =

* Fixed erroneous error message on multi-file uploads.
* Readme.txt content updated

= 3.2 =

* Added file renaming functionality
* Added new shortcode options: showthumb, showsize, showdate, showactions and showheader to allow customization of the table appearance.
* Added Front-Side file deletion and show/hide table header options
* Added ability to either show or hide the File Actions links ( Open | Download ) to the List Settings
* Improved file name sanitizing to account for non-english characters
* Added custom action hook, eeSFL_UploadCompleted, to allow developers to customize post-upload behavior
* Fixed a bug where file downloading used the wrong MIME type
* Fixed broken upload notifications
* The plugin now supports Folders with purchase of a plugin extension. Check out the Folders tab

= 3.1.2 =

* Bug Fixes
* Fixed an error in the shortcode instructions
* Improved support form
* Merged uploader and list containers

= 3.1.1 =

* Thumbnails - Optionally show thumbnails for videos and image files, or file type icons for documents.
* FFmpeg Support - Allows automatic thumbnail creation for videos.
* File List Table Sorting - Built-in file list table sorting. Click on a column heading to sort it.
* Forced Download Link - Allows for forced downloading of a file, rather than relying on the default browser action.
* Choose File Types - Fully customize the types of files you want users to be able to upload.
* Date Format - The file date format now uses the format selected in your Wordpress General Settings.
* Custom Upload Folder - Specify whatever folder you like for users to upload files.
* Additional Viewing Restrictions - Limit viewing the list and/or uploading only to Admins.
* Shortcode Attributes - Hide the list or the uploader on a per page/post basis using [eeSFL showlist="NO" allowuploads="NO"]
* Improved Admin Dashboard / Simplified Menu
* Extensive under-the-hood updates and improvements

= 3.0.6 =
* Minor Bug Fix

= 3.0.5 =
* Minor Bug Fix

= 3.0.4 = 

* Added ability to hide the size or date columns
* Improved the list loading speed for large lists
* Improved file uploader process and speed
* Added ability to limit the number of files uploaded
* Improved upload email notifications
* Added ability to allow uploading to only logged-in users
* Added ability to show the list to only logged-in users
* Improved compatibility with password protected directories
* Revamped and greatly improved Admin Panel
* Improved usage instructions and added integrated email support form
* Added Internationalization (l18n), with several default translations

= 2.0.8 =

* Fixed activation bug *
* Improved directory creation failure fallback
* Fixed bug where no uploader would appear in the admin area if "None" was selected for the front-side

= 2.0.7 = 

* Added multi-file uploading option
* Added optional front-side user input form
* Added configurable upload directory option
* Added File List sorting options
* Improved HTML display and design
* Improved email notifications
* Added ability to send notifications to more than one address
* Many minor bug fixes and basic improvements

= 1.0.4 =

* Added settings option to add the username of the uploader to the file name.
* Improved date format
* Added support for the Table Sorter plugin by Farhan Noor, so the file list may be sortable by name or date
* Minor bug fixes

= 1.0.3 = 

* HTML Bug Fix
* Improved error reporting

= 1.0.2 =

* Fixed issue where uploading failed from Admin page
* Improved error reporting


= 1.0.1 =

* First Update - Minor Changes

= 1.0 =

* FIRST RELEASE VERSION - Previous versions were non-Wordpress.
