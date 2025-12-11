file(REMOVE_RECURSE
  "client/main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appclient_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
