<?php
  require_once('../Archive/Tar.php');
  session_start();
  $message = '';

  if (isset($_FILES['uploadedFile']) && $_FILES['uploadedFile']['error'] === UPLOAD_ERR_OK)
  {
    $fileTmpPath = $_FILES['uploadedFile']['tmp_name'];
    $fileName = $_FILES['uploadedFile']['name'];
    $fileSize = $_FILES['uploadedFile']['size'];
    $fileType = $_FILES['uploadedFile']['type'];
    $fileExtension = strtolower(end(explode(".", $fileName)));
    $uploadFolderName = md5(time() . $fileName);

    if ($fileExtension == 'tar')
    {
      $archive = new Archive_Tar($fileTmpPath);
      $archive->extract();

      if(is_null($archive->error_object))
      {
        $message = 'Submission received.<br/><br/>';
        $message .= implode('<br/>', array_map(function ($entry) {
          return '&bull; '.$entry['filename'];
        }, $archive->listContent()));
        $message .= '<br/>';
      }
      else
      {
        $message = $archive->error_object->getMessage();
      }
    }
    else
    {
      $message = 'Upload failed. Allowed file types: tar';
    }
  }
  else
  {
    $message = 'There is some error in the file upload. Please check the following error.<br>';
    $message .= 'Error:' . $_FILES['uploadedFile']['error'];
  }
$_SESSION['message'] = $message;
header("Location: /index.php");