
BUILD_DIR = "build"


task :default => :build

task :build do
  unless FileTest::exists?( BUILD_DIR + "/g3log/src/g3log-build/libg3logger.a" )
    sh "cd %s && make deps" % BUILD_DIR
  end

  sh "cd %s && make" % BUILD_DIR
end

task :test do
  sh "cd %s && make unit_test" % BUILD_DIR
end

task :bootstrap do
opencv_24_dir = "/opt/opencv-2.4/share/OpenCV"

  FileUtils::mkdir BUILD_DIR unless FileTest::directory? BUILD_DIR
  sh "cd %s && OpenCV_DIR=#{opencv_24_dir} cmake -D CMAKE_BUILD_TYPE:STRING=\"Debug\" " \
        "-D LOCAL_LIBACTIVE_OBJECT=../libactive_object .." % BUILD_DIR
end
