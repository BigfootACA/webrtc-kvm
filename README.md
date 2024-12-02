# WebRTC IP-KVM

Keyboard, Video, Mouse via WebRTC

## Configuration document

```yaml
# HTTP API Server
http:
  # HTTP server port
  port: 2345
  # HTTP Auth method (none, password, pam)
  auth: pam
  pam:
    # PAM service
    service: system-auth
    # Allowed users
    allowed:
    - root
    # Show username input
    username: no
    # Default username
    default: root

# USB Gadget settings
usb:
  # USB Device Controller
  udc: usbip-vudc.0
  # USB Gadget ConfigFS Name
  gadget: webrtc-kvm
  # USB Vendor ID
  id_vendor: 0x1D6B
  # USB Product ID
  id_product: 0x0104
  # USB Manufacturer Name
  manufacturer: ClassFun
  # USB Product Name
  product: WebRTC KVM

# Video related settings
video:
  streams:
    - id: hdmi-cap
      name: HDMI Capture
      # Use Video4Linux2
      driver: v4l2-cap
      # Video4Linux2 Capture Streaming device
      device: /dev/video-cap0
      fourcc: NV12

    - id: video-encoder
      name: H.264 Encoder
      # Use Video4Linux2
      driver: v4l2-m2m
      # Video4Linux2 Memory-to-Memory encoder device
      device: /dev/video-enc0
      fourcc: H264

    - id: webrtc
      name: WebRTC Streamer
      driver: webrtc

  # Video stream pipes
  links:
    # From HDMI Capture to H.264 Encoder
    - source: hdmi-cap
      sink: video-encoder
      type: dmabuf

    # From H.264 Encoder to WebRTC Streamer
    - source: video-encoder
      sink: webrtc
      type: pointer

  # Auto stop stream when all clients disconnected
  auto_stop: true
  # Video Width
  width: 1920
  # Video Height
  height: 1080
  # Video Framerate (Frames per second)
  fps: 60
```

## Support status

### Streaming Formats

#### Video

| Type         | Status      |
|--------------|-------------|
| H.264 / AAC  | ✅ WORKS     |
| H.265 / HEVC | ⚠️ UNTESTED |
| VP8          | ⛔ TODO      |
| VP9          | ⛔ TODO      |
| AV1          | ⛔ TODO      |

#### Audio

All audio streaming are not implements now

### Backends

#### Video Capture

| Type                  | Status     |
|-----------------------|------------|
| Video4Linux2 Capture  | ✅ WORKS    |
| X11 Grab              | ⛔ TODO     |
| VNC Client            | ⛔ TODO     |
| DRM Capture           | ⛔ TODO     |
| Framebuffer           | ⛔ TODO     |

#### Video Encoder

| Type                           | Status     |
|--------------------------------|------------|
| Video2Linux2 Memory-to-Memory  | ✅ WORKS    |
| VA-API                         | ⛔ TODO     |
| VDPAU                          | ⛔ TODO     |
| CedarX (Allwinner)             | ⛔ TODO     |
| RKMPP (Rockchip)               | ✅ WORKS    |
| FFmpeg AVCodec                 | ⛔ TODO     |

#### Video Transformer

| Type                           | Status     |
|--------------------------------|------------|
| FFmpeg SWScale                 | ⚠️ UNTESTED |
| RGA (Rockchip)                 | ✅ WORKS   |

#### Keyboard and Mouse

| Type                              | Status      |
|-----------------------------------|-------------|
| USB Gadget HID Keyboard           | ✅ WORKS     |
| USB Gadget HID Absolute Mouse     | ✅ WORKS     |
| USB Gadget HID Relative Mouse     | ✅ WORKS     |
| USB Gadget HID Touch Screen       | ✅ WORKS     |
| Bluetooth peripheral Keyboard     | ⛔ TODO      |
| Bluetooth peripheral Mouse        | ⛔ TODO      |
| Bluetooth peripheral Tablet       | ⛔ TODO      |
| Bluetooth peripheral Touch Screen | ⛔ TODO      |
| Linux Userspace HID (uhid)        | ⛔ TODO      |

#### Alternative functions

| Type                           | Status     |
|--------------------------------|------------|
| USB Gadget MTP Folder          | ⛔ TODO     |
| USB Gadget Mass-Storage        | ⛔ TODO     |
| USB Gadget WebUSB Direct       | ⛔ TODO     |

### Tested devices

| Name                | SoC               | Capture Device      | Capture Interface | Kernel           | Video                |
|---------------------|-------------------|---------------------|-------------------|------------------|----------------------|
| Ant-C v3 IP-KVM     | Allwinner V3      | ITE IT6616FN        | HDMI to MIPI-CSI  | 6.6.2 (Mainline) | H.264 1920x1080@60hz |
| Raspberry Pi 3B     | Broadcom BCM2837  | Toshiba TC358743    | HDMI to MIPI-CSI  | 6.6.44           | H.264 1920x1080@50hz |
| Raspberry Pi 4B     | Broadcom BCM2711  | Toshiba TC358743    | HDMI to MIPI-CSI  | 6.6.44           | H.264 1920x1080@50hz |
| Raspberry Pi 4B     | Broadcom BCM2711  | MacroSilicon MS2130 | HDMI to USB 3.0   | 6.6.44           | H.264 1920x1080@60hz |
| Rockchip RK3588 EVB | Rockchip RK3588   | Rockchip HDMI-RX    | HDMI              | 5.10.66          | H.264 1920x1080@60hz |

Tested config files for devices: [configs](configs)
