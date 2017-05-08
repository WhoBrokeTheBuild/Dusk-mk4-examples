SET(FOLDER "examples")

INCLUDE("ExampleProject")

ADD_SUBDIRECTORY(triangle)
ADD_SUBDIRECTORY(flat)
ADD_SUBDIRECTORY(phong)
ADD_SUBDIRECTORY(textured)
ADD_SUBDIRECTORY(wave)
ADD_SUBDIRECTORY(cameras)

IF(UNIX)
  ADD_CUSTOM_TARGET(
    format-examples
    DEPENDS format-triangle format-flat format-phong format-textured format-wave format-cameras
  )
ENDIF()
