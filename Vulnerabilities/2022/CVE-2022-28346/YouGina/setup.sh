mv cve202228346/urls.py .
mv cve202228346/settings.py .
sudo docker-compose run web django-admin startproject cve202228346 .
mv -f urls.py cve202228346/
mv -f settings.py cve202228346/
