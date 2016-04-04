{
  "targets": [
    {
      "target_name": "GameOfRisk",
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-std=c++11",
          "-stdlib=libc++"
        ],
        "MACOSX_DEPLOYMENT_TARGET": "10.10"
      },
      "cflags": [
        "-std=c++11",
        "-stdlib=libc++"
      ],
      "sources": [
        "native/GameOfRisk.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
