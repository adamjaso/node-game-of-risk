{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "native/addon.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
