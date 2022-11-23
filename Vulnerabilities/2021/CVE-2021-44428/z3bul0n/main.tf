terraform {
  required_providers {
    docker = {
      source  = "kreuzwerker/docker"
      version = "2.15.0"
    }
  }
}

provider "docker" {
  host = "unix:///var/run/docker.sock"
}

resource "docker_image" "web" {
  name = "log4j_web:latest"
}

resource "docker_image" "exploit" {
  name = "log4j_exploit:latest"
}

resource "docker_image" "payload" {
  name = "log4j_payload:latest"
}

resource "docker_image" "netcat" {
  name = "log4j_netcat:latest"
}


resource "docker_container" "web" {
  image = docker_image.web.latest
  name  = "web"

  ports {
    internal = 8080
    external = 8080
  }

}

resource "docker_container" "exploit" {
  image = docker_image.exploit.latest
  name  = "exploit"

  ports {
    internal = 1389
    external = 1389
  }
}

resource "docker_container" "netcat" {
  image = docker_image.netcat.latest
  name  = "netcat"

  ports {
    internal = 9001
    external = 9001
  }

}

resource "docker_container" "payload" {
  image = docker_image.payload.latest
  name  = "payload"

  ports {
    internal = 80
    external = 80
  }
}
