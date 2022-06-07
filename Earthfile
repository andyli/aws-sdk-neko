VERSION 0.6
ARG UBUNTU_RELEASE=bionic
FROM mcr.microsoft.com/vscode/devcontainers/base:0-$UBUNTU_RELEASE

ARG DEVCONTAINER_IMAGE_NAME_DEFAULT=ghcr.io/andyli/aws_sdk_neko_devcontainer

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

ARG --required TARGETARCH

ARG WORKDIR=/workspace
RUN mkdir -m 777 "$WORKDIR"
WORKDIR "$WORKDIR"

devcontainer-library-scripts:
    RUN curl -fsSLO https://raw.githubusercontent.com/microsoft/vscode-dev-containers/main/script-library/common-debian.sh
    RUN curl -fsSLO https://raw.githubusercontent.com/microsoft/vscode-dev-containers/main/script-library/docker-debian.sh
    SAVE ARTIFACT --keep-ts *.sh AS LOCAL .devcontainer/library-scripts/

devcontainer-base:
    # Avoid warnings by switching to noninteractive
    ENV DEBIAN_FRONTEND=noninteractive

    ARG INSTALL_ZSH="false"
    ARG UPGRADE_PACKAGES="true"
    ARG ENABLE_NONROOT_DOCKER="true"
    ARG USE_MOBY="false"
    COPY .devcontainer/library-scripts/common-debian.sh .devcontainer/library-scripts/docker-debian.sh /tmp/library-scripts/
    RUN apt-get update \
        && /bin/bash /tmp/library-scripts/common-debian.sh "${INSTALL_ZSH}" "${USERNAME}" "${USER_UID}" "${USER_GID}" "${UPGRADE_PACKAGES}" "true" "true" \
        && /bin/bash /tmp/library-scripts/docker-debian.sh "${ENABLE_NONROOT_DOCKER}" "/var/run/docker-host.sock" "/var/run/docker.sock" "${USERNAME}" "${USE_MOBY}" \
        # Clean up
        && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* /tmp/library-scripts/

    # Setting the ENTRYPOINT to docker-init.sh will configure non-root access 
    # to the Docker socket. The script will also execute CMD as needed.
    ENTRYPOINT [ "/usr/local/share/docker-init.sh" ]
    CMD [ "sleep", "infinity" ]

    # Configure apt and install packages
    RUN apt-get update \
        && apt-get install -qqy --no-install-recommends apt-utils dialog 2>&1 \
        && apt-get install -qqy --no-install-recommends \
            iproute2 \
            procps \
            sudo \
            bash-completion \
            build-essential \
            cmake \
            curl \
            wget \
            software-properties-common \
            direnv \
            tzdata \
            # build deps
            uuid-dev \
            libcurl4-gnutls-dev \
            libssl1.0-dev \
            zlib1g-dev \
            # install docker engine for using `WITH DOCKER`
            docker-ce \
        # install haxe
        && add-apt-repository ppa:haxe/haxe4.2 \
        && apt-get install -qqy --no-install-recommends neko neko-dev haxe=1:4.2.* \
        # install a recent git
        && add-apt-repository ppa:git-core/ppa \
        && apt-get install -qqy --no-install-recommends git \
        #
        # Clean up
        && apt-get autoremove -y \
        && apt-get clean -y \
        && rm -rf /var/lib/apt/lists/*

    # Switch back to dialog for any ad-hoc use of apt-get
    ENV DEBIAN_FRONTEND=

# Usage:
# RUN /aws/install
awscli:
    FROM +devcontainer-base
    RUN curl -fsSL "https://awscli.amazonaws.com/awscli-exe-linux-$(uname -m).zip" -o "/tmp/awscliv2.zip" \
        && unzip -qq /tmp/awscliv2.zip -d / \
        && rm /tmp/awscliv2.zip
    SAVE ARTIFACT /aws

# Usage:
# COPY +doctl/doctl /usr/local/bin/
doctl:
    ARG TARGETARCH
    ARG DOCTL_VERSION=1.66.0
    RUN curl -fsSL "https://github.com/digitalocean/doctl/releases/download/v${DOCTL_VERSION}/doctl-${DOCTL_VERSION}-linux-${TARGETARCH}.tar.gz" | tar xvz -C /usr/local/bin/
    SAVE ARTIFACT /usr/local/bin/doctl

# Usage:
# COPY +earthly/earthly /usr/local/bin/
# RUN earthly bootstrap --no-buildkit --with-autocomplete
earthly:
    FROM +devcontainer-base
    ARG --required TARGETARCH
    RUN curl -fsSL https://github.com/earthly/earthly/releases/download/v0.6.15/earthly-linux-${TARGETARCH} -o /usr/local/bin/earthly \
        && chmod +x /usr/local/bin/earthly
    SAVE ARTIFACT /usr/local/bin/earthly

rclone:
    FROM +devcontainer-base
    ARG --required TARGETARCH
    ARG RCLONE_VERSION=1.57.0
    RUN curl -fsSL "https://downloads.rclone.org/v1.57.0/rclone-v1.57.0-linux-${TARGETARCH}.zip" -o rclone.zip \
        && unzip -qq rclone.zip \
        && rm rclone.zip
    SAVE ARTIFACT rclone-*/rclone

devcontainer:
    FROM +devcontainer-base

    # Install earthly
    COPY +earthly/earthly /usr/local/bin/
    RUN earthly bootstrap --no-buildkit --with-autocomplete

    # AWS cli
    COPY +awscli/aws /aws
    RUN /aws/install

    # doctl
    COPY +doctl/doctl /usr/local/bin/

    # Install rclone
    COPY +rclone/rclone /usr/local/bin/

    USER $USERNAME

    # install haxelibs
    COPY +haxelibs/.haxelib .haxelib
    VOLUME "$WORKDIR/.haxelib"

    # Config direnv
    COPY --chown=$USER_UID:$USER_GID .devcontainer/direnv.toml /home/$USERNAME/.config/direnv/config.toml

    # Config bash
    RUN echo 'eval "$(direnv hook bash)"' >> ~/.bashrc \
        && echo "complete -C '/usr/local/bin/aws_completer' aws" >> ~/.bashrc \
        && echo 'source <(doctl completion bash)' >> ~/.bashrc

    USER root

    ARG GIT_SHA
    ENV GIT_SHA="$GIT_SHA"
    ARG IMAGE_NAME="$DEVCONTAINER_IMAGE_NAME_DEFAULT"
    ARG IMAGE_TAG="master"
    ARG IMAGE_CACHE="$IMAGE_NAME:$IMAGE_TAG"
    SAVE IMAGE --cache-from="$IMAGE_CACHE" --push "$IMAGE_NAME:$IMAGE_TAG"

haxelibs:
    FROM +devcontainer-base
    USER $USERNAME
    COPY *.hxml .
    RUN haxelib newrepo && haxelib install all --always
    SAVE ARTIFACT .haxelib

build:
    FROM +devcontainer
    COPY +haxelibs/.haxelib .haxelib
    COPY aws-sdk-cpp aws-sdk-cpp
    COPY src src
    COPY test test
    COPY lib lib
    COPY CMakeLists.txt *.hxml .
    RUN cmake .
    RUN cmake --build .
    SAVE ARTIFACT bin/*

package:
    COPY lib lib
    COPY src src
    COPY CMakeLists.txt haxelib.json README.md .
    COPY --platform=linux/amd64 +build/aws.ndll ndll/Linux64/aws.ndll
    SAVE ARTIFACT *

package-zip:
    FROM +package
    RUN zip -r aws-sdk-neko.zip *
    SAVE ARTIFACT aws-sdk-neko.zip AS LOCAL bin/aws-sdk-neko.zip

ghcr-login:
    LOCALLY
    RUN echo "$GITHUB_CR_PAT" | docker login ghcr.io -u "$GITHUB_USERNAME" --password-stdin

ci-devcontainer:
    ARG --required GIT_SHA
    ARG --required GIT_REF_NAME
    BUILD +devcontainer \
        --GIT_SHA="$GIT_SHA" \
        --IMAGE_TAG="$GIT_SHA" \
        --IMAGE_TAG="$GIT_REF_NAME"
