resource "digitalocean_ssh_key" "default" {
  name       = "My SSH Key"
  public_key = file(var.ssh_key_path)
}

