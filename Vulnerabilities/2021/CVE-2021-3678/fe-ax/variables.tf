# Cloud provider
variable "do_token" {}

variable "domain" {
  type = object({
    domain    = string
    subdomain = string
  })
  description = "The domain information"
}

variable "prefix" {
  type        = string
  description = "Global prefix"
}

variable "cloud_extra_cluster" {
  type = object({
    count  = number
    image  = string
    region = string
    size   = string
  })
  description = "The settings for extra-cluster VM's"
}

variable "cloud_local_cluster" {
  type = object({
    count  = number
    image  = string
    region = string
    size   = string
  })
  description = "The settings for local-cluster VM's"
}