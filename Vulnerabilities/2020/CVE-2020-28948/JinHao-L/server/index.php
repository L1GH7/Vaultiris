<?php
  session_start();
?>
<!DOCTYPE html>
<html>
  <head>
    <title>File Submission</title>
  </head>
  <body>
    <?php
      if (isset($_SESSION['message']) && $_SESSION['message'])
      {
        printf('<b>%s</b>', $_SESSION['message']);
        unset($_SESSION['message']);
      }
    ?>
    <form action="uploads/index.php" method="post" enctype="multipart/form-data">
      <div>
        <span>Zip your submissions and submit as a single tar file:</span>
        <input type="file" name="uploadedFile" />
      </div>

      <input type="submit" />
    </form>
  </body>
</html>