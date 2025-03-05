# Installation of Docker and AlmaLinux9 on Ubuntu 22.04 LTS

Docker is a powerful containerization platform that allows developers to package applications and their dependencies in lightweight and portable containers. These containers ensure consistency across different environments, making the application deployment more efficient and reliable. This guide focuses on installing AlmaLinux 9 in a Docker containers. AlmaLinux is a community-oriented enterprise-grade Linux distribution that derives from . We will guide you through the configuration of Docker and AlmaLinux 9 on Ubuntu 22.04 LTS system, ensuring smooth and practical experience. All the commands and shortcut reported in this guide, refers to an Ubuntu 22.04 LTS OS version.

## Minimum requirements

In order to install Docker, ensure the minimum requirements are met by your machine:
- Have an x86-64 system with Ubuntu 22.04, 24.04, or the latest non-LTS version. You can recover this information by entering this command in the terminal (`CTRL+ALT+T` to open it):
```
uname -a
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

## Docker installation

### Step 1 - Set up Docker's package repository 

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
they, respectively, refresh the list of available packages, install `ca-certificates` and `curl` packages, creates an `/etc/apt/keyrings/` directory with proper permissions, add Docker official GPG key, set up proper key permissions, add Docker official repository and update the package list again.

### Step 2 - Download the Docker DEB package and install it

To do this, browse to `https://docs.docker.com/desktop/` website. From the sidebar menu on the left, navigate to `Products`>`Docker desktop`>`Setup`>`Install`>`Linux`>`Ubuntu`. From here, reach the “Install Docker Desktop” section and download the DEB package linked in step 2. In this way, you will have a `~/Downloads/docker-desktop-amd64.deb` file. Then proceeds with its installation:
```
sudo apt-get update
sudo apt-get install ./Downloads/docker-desktop-amd64.deb
```

Note that, at the end of installation, an error may show up:
```
N: Download is performed unsandboxed as root, as file '/home/user/Downloads/docker-desktop.deb' couldn't be accessed by user '_apt'. - pkgAcquire::Run (13: Permission denied)
```
however, you can ignore this message. If the installation ended successfully, by giving the command `docker --version` you should see the installed version of Docker (`Docker version XX.X.X, build XXXX`).

## AlmaLinux9 installation

### Step 1 - Download the AlmaLinux9 image

Once you installed Docker, you can launch it either via terminal using `systemctl --user start docker-desktop` or by looking at its tray icon if you are using a GNOME Desktop Environment. First of all, you should get the AlmaLinux9 image. This can be done opening a terminal within Docker (from the bottom-right of the Docker window) and use this command:
```
docker pull almalinux:9
```
you can find the image in the “Images” section of the sidebar menu on the left-hand-side of the Docker window. 

### Step 2 - Create a new container

To create a new container in which launch this image, you can run this command:
```
docker run --name almaLinux AlmaLinux:9
```
then you will find it in the “Containers” section in the usual sidebar menu mentioned before. To launch it, you can press the play button under “Actions”. In this same section, you can open a terminal and verify we are actually using an AlmaLinux distribution entering:
```
cat /etc/os-release
```
that will give an output like this:
```
NAME="AlmaLinux"
VERSION="9.5 (Teal Serval)"
```

To stop the container, you can press the stop button on the top-right corner of the Docker window. 

Now you are running AlmaLinux 9 via Docker!
