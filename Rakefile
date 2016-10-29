
task :default => "debug:test"

@cmake_opts = ['-D BUILD_UNIT_TESTS:BOOL=True']
load 'config.rb' if FileTest::exists? 'config.rb'

['Debug','Release'].each { |build_type|
  namespace build_type.downcase.to_sym do
    build_dir = ENV['BUILD_DIR'] || "build-#{build_type}"

    task :build do
      unless FileTest::directory? build_dir
        FileUtils::mkdir build_dir
        sh "cd %s && cmake -D CMAKE_BUILD_TYPE:STRING=\"%s\" %s  .." % [build_dir, build_type, @cmake_opts.join(' ')]
      end

      if !FileTest::exists? build_dir + "/g3log/src/g3log-build/libg3logger.a"
        sh "cd %s && make deps" % build_dir
      end

      sh "cd %s && make" % build_dir
    end

    task :test => :build do
      sh "cd %s && make unit_test" % build_dir
    end
  end
}

namespace :dependencies do
  task :linux do
    sh "sudo apt-get install -y cmake libopencv-dev libtclap-dev libboost-all-dev"
  end

  task :osx do
    sh "brew update"
    sh "brew tap homebrew/science"
    sh "brew install cmake homebrew/science/opencv"
  end
end
