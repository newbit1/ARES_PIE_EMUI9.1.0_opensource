# Huawei Y Max Android PIE Kernel v4.4.153 Opensource from Huawei
### [newbit @ xda-developers](https://forum.xda-developers.com/m/newbit.1350876/)
Opensource Code for the Huawei Y Max (ARES) Android PIE Kernel

### Get some packages
`sudo apt-get install build-essential libssl-dev libncurses5-dev bison flex libqt4-dev pkg-config meld`

### Get the Toolchain
`./configure_gcc_toolchain`

### Exports
`source .exports`

### Compile it
`make ARCH=arm64 distclean`\
`make ARCH=arm64 merge_sdm660_64_defconfig`\
`make ARCH=arm64 FIRMWARE_sdm660_64_defconfig`\
`make ARCH=arm64 xconfig`\
`time make ARCH=arm64 -j$(nproc)`

### Links
* [consumer huawei opensource](https://consumer.huawei.com/en/opensource)
* [ARES_PIE_EMUI9.1.0_opensource.tar.gz](http://download-c1.huawei.com/download/downloadCenter?downloadId=100885&version=436618&siteCode=worldwide)
* [Prebuild GCC Toolchain Android NDK 17c](https://dl.google.com/android/repository/android-ndk-r17c-linux-x86_64.zip)

### Merged qcom-opensource
`TAG=LA.UM.7.2.r1-06700-sdm660.0`\
`git remote add qcacld https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/wlan/qcacld-3.0`\
`git fetch qcacld $TAG`\
`git merge -s ours --no-commit FETCH_HEAD --allow-unrelated-histories`\
`git read-tree --prefix=kernel/drivers/staging/qcacld-3.0 -u FETCH_HEAD`\
`git commit --no-edit`\
`git remote add fw-api https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/wlan/fw-api`\
`git fetch fw-api $TAG`\
`git merge -s ours --no-commit FETCH_HEAD --allow-unrelated-histories`\
`git read-tree --prefix=kernel/drivers/staging/fw-api -u FETCH_HEAD`\
`git commit --no-edit`\
`git remote add qca-wifi-host-cmn https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/wlan/qca-wifi-host-cmn`\
`git fetch qca-wifi-host-cmn $TAG`\
`git merge -s ours --no-commit FETCH_HEAD --allow-unrelated-histories`\
`git read-tree --prefix=kernel/drivers/staging/qca-wifi-host-cmn -u FETCH_HEAD`\
`git commit --no-edit`
