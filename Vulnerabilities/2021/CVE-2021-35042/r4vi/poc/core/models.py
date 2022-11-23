from django.db import models


class Tag(models.Model):
    name = models.TextField()


class Things(models.Model):
    name = models.TextField()
    tags = models.ManyToManyField(Tag)
