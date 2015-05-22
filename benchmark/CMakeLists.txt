# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Required packages, gems and caveats
##############################################################################
if(NOT ${RUBY_FOUND})
    message(WARNING
        "Ruby >= 2.1 was not found; the benchmarks will be unavailable.")
    return()
endif()

# Check for the 'ruby-progressbar' and 'tilt' gems
execute_process(COMMAND ${RUBY_EXECUTABLE} -r ruby-progressbar -r tilt -e ""
                RESULT_VARIABLE __BOOST_HANA_MISSING_GEMS
                OUTPUT_QUIET ERROR_QUIET)
if(${__BOOST_HANA_MISSING_GEMS})
    message(WARNING
        "The 'ruby-progressbar' and/or 'tilt' gems were not found; "
        "the benchmarks will be unavailable."
        "Use `gem install ruby-progressbar tilt` to install the missing gems.")
    return()
endif()


##############################################################################
# Setup custom functions, master targets and file lists for the benchmarks
##############################################################################
add_custom_target(benchmarks COMMENT "Generate all the benchmarks.")

# Configure the measure.rb script
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/measure.in.rb     # input
               ${CMAKE_CURRENT_BINARY_DIR}/measure.rb @ONLY) # output

# The list of all the '.in.erb.json' templates
file(GLOB_RECURSE BOOST_HANA_IN_ERB_JSON_FILES "*.in.erb.json")

#   boost_hana_add_benchmark(<filename.in.erb.json>)
#
# Given a `/path/to/file.in.erb.json` in the `/benchmark/` directory
# (or a subdirectory thereof), `boost_hana_add_benchmark` adds a new target
# named `benchmark.path.to.file.json` which generates a `.json` file suitable
# for drawing a chart.
function(boost_hana_add_benchmark in_erb_json)
    # Given a '/benchmark/something/x.in.erb.json' file, we create the path
    # to '/build_dir/benchmark/something/x.erb.json'. We then configure the
    # first file and store the result in the file whose path we just generated.
    file(RELATIVE_PATH erb_json ${CMAKE_CURRENT_SOURCE_DIR} ${in_erb_json})
    set(erb_json "${CMAKE_CURRENT_BINARY_DIR}/${erb_json}")
    string(REPLACE ".in.erb.json" ".erb.json" erb_json ${erb_json})
    configure_file("${in_erb_json}" "${erb_json}" @ONLY)

    # We store the '/benchmark/something/' directory in the 'dir' variable
    # for later use.
    get_filename_component(dir "${in_erb_json}" DIRECTORY)

    # We store the path of '/build_dir/something/x.json' in the 'json' variable.
    string(REPLACE ".erb.json" ".json" json ${erb_json})

    # We add a custom command generating the '/build_dir/benchmark/something/x.json'
    # file from the 'x.erb.json' file we just configured. Generating this '.json'
    # file is what gathers the benchmark data.
    add_custom_command(OUTPUT "${json}"
        COMMAND ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "json = Tilt::ERBTemplate.new('${erb_json}').render"
            -e "File.open('${json}', 'w') { |f| f.write(json) } "
        WORKING_DIRECTORY ${dir}
        DEPENDS "${erb_json}"
        VERBATIM
        COMMENT "Generating benchmarks for ${json}"
    )

    # Generate a target name for generating the 'x.json' file. Since there is
    # a one-to-one correspondance between the 'x.json' file and the original
    # 'x.in.erb.json' file, we use the 'benchmark.something.x.json' name for
    # this target.
    file(RELATIVE_PATH target ${CMAKE_CURRENT_SOURCE_DIR} "${in_erb_json}")
    string(REPLACE ".in.erb.json" ".json" target "${target}")
    string(REPLACE "/" "." target "${target}")
    add_custom_target(benchmark.${target} DEPENDS "${json}")
    add_dependencies(benchmarks benchmark.${target})


    # For each benchmark, we also create a test that can be run using CTest.
    # This test runs the benchmark, but only with the lowest and highest values
    # in the benchmarked input ranges. This is just to make sure the benchmarks
    # are not broken over time.
    #
    # Note that we purposefully don't use `boost_hana_add_test` here.
    add_test(benchmark.${target}
        COMMAND ${CMAKE_COMMAND} -E env BOOST_HANA_CHECK_BENCHMARKS=true
        ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "Tilt::ERBTemplate.new('${erb_json}').render"
        WORKING_DIRECTORY ${dir}
    )
endfunction()


##############################################################################
# Include directories
##############################################################################
include_directories(${Boost.Hana_SOURCE_DIR}/include)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if (MPL11_FOUND)
    include_directories(${MPL11_INCLUDE_DIR})
endif()

if (Meta_FOUND)
    include_directories(${Meta_INCLUDE_DIR})
endif()


##############################################################################
# Add the special measure file and targets
#
# The 'benchmark.measure.compilation' target is a special target that
# compiles the measure.cpp file and outputs compilation statistics.
#
# Similarly, the 'benchmark.measure.runtime' target is a special target that
# compiles the measure.cpp file, runs the resulting program and then
# outputs execution time statistics.
#
# The measure.cpp file should always be left empty. The intended usage is to
# put some code in the file, benchmark it and then put that code somewhere
# else. In particular, note that the benchmarks added below use the measure
# file to perform their benchmarking, and launching the benchmarks will wipe
# anything from the `measure.cpp` file.
##############################################################################
boost_hana_add_executable(benchmark.measure.compilation EXCLUDE_FROM_ALL measure.cpp)
set_target_properties(benchmark.measure.compilation
    PROPERTIES RULE_LAUNCH_COMPILE "${CMAKE_CURRENT_BINARY_DIR}/measure.rb")
add_custom_target(benchmark.measure.runtime
    ${CMAKE_CURRENT_BINARY_DIR}/benchmark.measure.compilation)


##############################################################################
# Add the benchmarks
##############################################################################
foreach(erb_json IN LISTS BOOST_HANA_IN_ERB_JSON_FILES)
    boost_hana_add_benchmark(${erb_json})
endforeach()


##############################################################################
# Setup targets to automatically upload the benchmarks.
#
# We have a branch named 'datasets'. This branch contains nothing, except
# directories and JSON files with the following layout:
#   debug/
#       compiler1/
#           benchmark1.json
#           ...
#           benchmarkN.json
#       ...
#       compilerN/
#           benchmark1.json
#           ...
#           benchmarkN.json
#   release/
#       ...
#
#   default/
#       ...
#
# The `debug`, `release` or `default` are what we call the `BUILD_TYPE_SLUG`
# below, and it corresponds to the CMake configuration under which the
# benchmarks were run.
#
# The `compilerN` subdirectories are what we call the `COMPILER_SLUG` below,
# ad it corresponds to the compiler used to compile the benchmarks.
#
# The `benchmarkN.json` files are those generated by the targets we set up
# just above. The 'datasets.clone' target we define below allows cloning
# the 'dataset' branch to the 'benchmark/dataset' subdirectory of the project,
# or pulling the latest revisions if the clone already exists. This directory
# is ignored by git to avoid polluting the index. Then, the 'datasets.clean'
# target allows removing the subdirectory associated to the current build type
# and compiler, and the 'datasets.copy' target allows populating that
# subdirectory with the newly built benchmarks. Finally, the 'datasets.commit'
# target allows committing the newly built benchmarks to the 'datasets' branch,
# and the 'datasets.push' target allows pushing those changes to the official
# repository. Of course, write access is needed to push the changes.
#
# Most of this is only used by our continuous integration scripts, but it is
# also possible to do it manually.
##############################################################################
if (NOT GIT_FOUND)
    message(WARNING
        "Git was not found; the various 'datasets.*' targets will be unavailable.")
    return()
endif()

set(DATASETS_DIR "${CMAKE_SOURCE_DIR}/benchmark/datasets")
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}" COMPILER_SLUG)
if (CMAKE_BUILD_TYPE)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_SLUG)
else()
    set(BUILD_TYPE_SLUG "default")
endif()

add_custom_target(datasets.clone
    COMMAND ${RUBY_EXECUTABLE}
    -e "if File.directory?('${DATASETS_DIR}')                                                             "
    -e "  Dir.chdir('${DATASETS_DIR}')                                                                    "
    -e "  `${GIT_EXECUTABLE} pull`                                                                        "
    -e "else                                                                                              "
    -e "  `${GIT_EXECUTABLE} clone ${BOOST_HANA_ORIGIN_URL} --depth 1 --branch=datasets ${DATASETS_DIR}`  "
    -e "end                                                                                               "
    COMMENT "Cloning the 'datasets' branch into the 'benchmark/datasets' directory"
    VERBATIM
)

add_custom_target(datasets.clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DATASETS_DIR}/${COMPILER_SLUG}
    COMMENT "Removing ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}"
    DEPENDS datasets.clone
    VERBATIM
)

add_custom_target(datasets.copy
    COMMENT "Copying the generated benchmarks from ${CMAKE_CURRENT_BINARY_DIR} to benchmark/datasets"
    DEPENDS datasets.clean
)
foreach(benchmark IN LISTS BOOST_HANA_BENCHMARKS)
    add_custom_target(${benchmark}.copy
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${benchmark}.json ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}
        DEPENDS ${benchmark} datasets.clean
        VERBATIM
    )
    add_dependencies(datasets.copy ${benchmark}.copy)
endforeach()

add_custom_target(datasets.commit
    COMMAND ${GIT_EXECUTABLE} add --all .
    COMMAND ${GIT_EXECUTABLE} commit --allow-empty -m "Update benchmarks to ${BOOST_HANA_CURRENT_COMMIT} for build type '${BUILD_TYPE_SLUG}' and compiler '${COMPILER_SLUG}'"
    WORKING_DIRECTORY ${DATASETS_DIR}
    COMMENT "Updating the datasets clone with the newest benchmarks"
    DEPENDS datasets.copy
    VERBATIM
)

add_custom_target(datasets.push
    COMMAND ${GIT_EXECUTABLE} push origin datasets
    WORKING_DIRECTORY ${DATASETS_DIR}
    COMMENT "Pushing the datasets branch to GitHub"
    DEPENDS datasets.commit
    VERBATIM
)
