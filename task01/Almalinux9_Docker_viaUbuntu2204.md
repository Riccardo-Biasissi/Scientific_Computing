# Installation of Docker and AlmaLinux 9 on Ubuntu 22.04 LTS

Docker is a powerful containerization platform that allows developers to package applications and their dependencies in lightweight and portable containers. These containers provide a consistent environment, streamlining application deployment and improving reliability. This guide focuses on installing AlmaLinux 9 in a Docker container. AlmaLinux is a community-oriented enterprise-grade Linux distribution that derives from . We will guide you through the configuration of Docker and AlmaLinux 9 on Ubuntu 22.04 LTS system. All the commands reported in this guide, refers to an Ubuntu 22.04 LTS.

## Minimum requirements

In order to install Docker, ensure the minimum requirements are met by your machine:
- Be running an x86-64 system with Ubuntu 22.04, 24.04, or the latest non-LTS version. To recover such information, enter this command in the terminal:
```
uname -a
```
the output will be in the form:
```
Linux PC_name kernel_version build_version SMP kernel_compulation_date x86_64 x86_64 x86_64 GNU/Linux
```
this confirms that the system is x86-64
- Ensure your system has at least 4GB of RAM. To check, use:
```
free -h
```
alternatively, if htop is installed, you can use:
```
htop
```
which open an interactive window that displays several informations, including the RAM memory amount. To exit this interactive window, press `F10`. If htop is not installed, install it with:
```
sudo apt install htop
```
- Verify you have sudo access (administrative privileges) by running:
```
sudo -v
```
if this command runs without errors, you have the necessary permissions

## Docker installation

### Step 1 - Set up Docker package repository 

When installing Docker for the first time on a new host machine, we have to set up the Docker `apt` repository to ensure that we will install and update Docker from the official package source:

- Update the package list and install prerequisites:
```
sudo apt-get update
sudo apt-get install ca-certificates curl
```
- Create the /etc/apt/keyrings/ directory with correct permissions:
```
sudo install -m 0755 -d /etc/apt/keyrings
```
- Download and add Docker’s `GPG` key:
```
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
```
- Set correct permissions for the `GPG` key:
```
sudo chmod a+r /etc/apt/keyrings/docker.asc
```
- Add the Docker repository to `apt` sources and update the package list again to include it:
```
echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```

### Step 2 - Download and install the Docker `.deb` package

Download the Docker Desktop `.deb` package from the official Docker website: browse to `https://docs.docker.com/desktop/` website and, from the sidebar menu on the left, navigate to `Products`>`Docker desktop`>`Setup`>`Install`>`Linux`>`Ubuntu`. From here, reach the “Install Docker Desktop” section and download the DEB package (see image). 

![ezcv logo](https://github.com/Riccardo-Biasissi/Scientific_Computing/blob/main/task01/images/download_deb.png)

In this way, you will download the `docker-desktop-amd64.deb` file in `~/Downloads/` directory. To proceed with its installation, enter this command in the terminal:
```
sudo apt-get install ./Downloads/docker-desktop-amd64.deb
```

Note that, at the end of installation, an error may show up:
```
N: Download is performed unsandboxed as root, as file '/home/user/Downloads/docker-desktop.deb' couldn't be accessed by user '_apt'. - pkgAcquire::Run (13: Permission denied)
```
however, you can ignore this message since it does not interfere with the installation. Check whether or not the installation ended successfully, by using the command:
```
docker --version
```
in which case, you should see the installed version of Docker:
```
Docker version XX.X.X, build XXXX
```

Once Docker has been installed, you can launch it either via terminal:
```
systemctl --user start docker-desktop
```
or by clicking at its tray icon if you are using a GNOME Desktop Environment (if you want to use this option remember to install the desktop environment with `sudo apt install gnome-terminal` command). 

## AlmaLinux 9 installation

### Step 1 - Download the AlmaLinux 9 image

Open a terminal within Docker from the bottom-right of the Docker window and use this command:
```
docker pull almalinux:9
```
this will download the AlmaLinux 9 image. You will find the downloaded image in the “Images” section of the sidebar menu on the left-hand-side of the Docker window. 

### Step 2 - Create a new container

Create a new container in which launch our AlmaLinux 9 image, run this command:
```
docker run --name almaLinux_container AlmaLinux:9
```
you will find this container in the “Containers” section in the previously mentioned sidebar menu. Click on the “Containers” to see a list of all containers. To launch the freshly created `almalinux_container`, press the play button under “Actions”. Next to it, there is an option button from which you can open a terminal. 

Verify we are actually using an AlmaLinux distribution by entering in its terminal:
```
cat /etc/os-release
```
that will give an output like this:
```
NAME="AlmaLinux"
VERSION="9.5 (Teal Serval)"
...
...
```

To stop the container, click the Stop button in the top-right corner of the Docker window.

Congratulations, you are running AlmaLinux 9 via Docker!
