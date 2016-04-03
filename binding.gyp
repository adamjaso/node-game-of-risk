{
  "targets": [
    {
      "target_name": "GameOfRisk",
      "sources": [
        "native/GameOfRisk.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
