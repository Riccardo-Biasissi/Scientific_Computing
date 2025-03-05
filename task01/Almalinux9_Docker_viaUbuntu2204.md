# Installation of Docker and Almalinux9 on Ubuntu 22.04 LTS

Docker is a powerful containerization platform that allows developers to package applications and their dependencies in lightweight and portable containers. These containers ensure consistency across different environments, making the application deployment more efficient and reliable. This guide focuses on installing AlmaLinux 9 in a Docker containers. AlmaLinux is a community-oriented enterprise-grade Linux distribution that derives from . We will guide you through the configuration of Docker and AlmaLinux 9 on Ubuntu 22.04 LTS system, ensuring smooth and practical experience. All the commands and shortcut reported in this guide, refers to an Ubuntu 22.04 LTS OS version.

## Verify minimum requirements

In order to install Docker, ensure the minimum requirements are met by your machine:
- Have an x86-64 system with Ubuntu 22.04, 24.04, or the latest non-LTS version. You can recover this information by entering this command in the terminal (`CTRL+ALT+T` to open it):
```
$ uname -a
```
the output will be in the form:
```
Linux PC_name kernel_version build_version SMP kernel_compulation_date x86_64 x86_64 x86_64 GNU/Linux
```
clearly we are running an x86-64 Linux system
- Have at least 4GB of RAM. You can enter the `htop` command to open and interactive window that displays several informations, including the RAM memory amount. In the eventuality that `htop` is not installed, you can add it to your system using:
```
sudo apt update && sudo apt install htop
```
- For non-Gnome Desktop environments, `gnome-terminal` must be installed. This can be performed by entering this command in terminal:
```
sudo apt update && sudo apt install gnome-terminal
```

## Docker installation

### Set up Docker's package repository 

When installing Docker for the first time on a new host machine, we have to set up the Docker `apt` repository to ensure that we will install and update Docker from the official package source. This can be done using these commands:
```
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```
they, in order, refresh the list of available packages, install `ca-certificates` and `curl` packages, creates an `/etc/apt/keyrings/` directory with proper permissions, add Docker official GPG key, set up proper key permissions, add Docker official repository and update the package list again.

Download the `.deb` package for our reference OS. Browse to `https://www.docker.com/products/docker-desktop/` and select “Download for Linux”



