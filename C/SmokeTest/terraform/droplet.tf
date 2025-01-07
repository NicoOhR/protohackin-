resource "digitalocean_droplet" "docker_server" {
  image  = "ubuntu-20-04-x64"
  name   = "docker-server"
  region = "nyc3"
  size   = "s-1vcpu-1gb"

  ssh_keys = [digitalocean_ssh_key.default.fingerprint]

  connection {
    host        = self.ipv4_address
    user        = "root"
    private_key = file("~/.ssh/id_ed25519")
  }

  provisioner "remote-exec" {
    inline = [
      "apt-get update",
      "apt-get install -y docker.io",
      "systemctl start docker",
      "systemctl enable docker",
      "docker run -d -p 5566:5566 nicoor/smoke_test"
    ]
  }
}

