file(REMOVE_RECURSE
  "client/qml/Chat.qml"
  "client/qml/Chats.qml"
  "client/qml/Header.qml"
  "client/qml/LogRegWindow.qml"
  "client/qml/main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appclient_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
