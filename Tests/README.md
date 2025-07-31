# ToyEngine Testing Guide

## Overview

This document provides comprehensive testing guidelines for the ToyEngine C++ game engine. The testing infrastructure is designed to ensure reliability, performance, and maintainability of all engine subsystems.

## Testing Framework

### Google Test (gtest)
- **Industry standard C++ testing framework**
- **Comprehensive assertion macros** for various data types
- **Test fixtures** for setup/teardown of test environments
- **Parameterized tests** for testing with multiple input values
- **Mock support** via Google Mock for isolating dependencies
- **Performance testing** capabilities with timing utilities

### Integration with Build System
- Tests are built as a separate executable project
- Automatic dependency management through Premake5
- Debug and Release configurations supported
- Cross-platform compatibility (Windows primarily, Linux planned)

## Test Organization

### Directory Structure
```
Tests/
├── src/
│   ├── main.cpp                          # Test runner entry point
│   ├── utils/
│   │   └── test_fixtures.h               # Common test fixtures and utilities
│   ├── unit_tests/
│   │   ├── camera_tests.cpp              # Camera system unit tests
│   │   ├── shader_tests.cpp              # Shader system unit tests
│   │   ├── model_tests.cpp               # Model/Mesh system unit tests
│   │   └── application_tests.cpp         # Application layer unit tests
│   └── integration_tests/
│       └── renderer_integration_tests.cpp # Cross-subsystem integration tests
└── premake5.lua                          # Test project configuration
```

## Test Categories

### 1. Unit Tests
**Purpose**: Test individual components in isolation

#### Camera System Tests (`camera_tests.cpp`)
- **Matrix Calculations**: View and projection matrix generation
- **Movement Operations**: Camera positioning and orientation
- **Mouse Look**: Camera rotation from input
- **FOV Zoom**: Field of view adjustments
- **Performance**: Matrix calculation performance benchmarks

#### Shader System Tests (`shader_tests.cpp`)
- **Shader Creation**: Interface and lifecycle management
- **Uniform Management**: Setting shader uniforms of various types
- **Shader Library**: Storage and retrieval of shader objects
- **Mock Testing**: Testing without OpenGL context dependencies

#### Model System Tests (`model_tests.cpp`)
- **Vertex Structure**: Memory layout and data integrity
- **Mesh Creation**: Vertex and index buffer management
- **Material Properties**: PBR material parameter handling
- **Primitive Generation**: Basic geometric shape creation

#### Application System Tests (`application_tests.cpp`)
- **Layer Lifecycle**: Layer attachment, update, and detachment
- **Layer Stack**: Management of rendering and update order
- **Event Handling**: Event dispatch to appropriate layers
- **Memory Management**: Safe creation and destruction of layers

### 2. Integration Tests
**Purpose**: Test interactions between multiple subsystems

#### Renderer Integration Tests (`renderer_integration_tests.cpp`)
- **Render Pipeline**: Complete rendering workflow from camera to output
- **Scene Management**: Scene graph traversal and rendering
- **Model Rendering**: Integration of models, materials, and shaders
- **Performance**: Frame rate and rendering throughput testing

### 3. Test Fixtures
**Purpose**: Provide common setup and utilities for tests

#### Base Fixtures
- **ToyEngineTestBase**: Common setup for all tests
- **RendererTestFixture**: OpenGL context simulation and renderer utilities
- **ApplicationTestFixture**: Application and window system testing utilities
- **ModelTestFixture**: Asset loading and model processing utilities

## Running Tests

### Prerequisites
- **Visual Studio 2022** (or compatible C++ compiler)
- **CMake or Premake5** for project generation
- **Git** for dependency management

### Build and Execute
```bash
# Generate project files
vendor/premake/premake5.exe vs2022

# Build Tests project in Visual Studio
# Or use command line:
msbuild ToyEngine.sln -p:Configuration=Debug -t:Tests

# Run tests
bin/Debug-windows-x86_64/Tests/Tests.exe

# Run with specific filters
Tests.exe --gtest_filter="CameraTest.*"
Tests.exe --gtest_filter="*Performance*"
```

### Test Output
```
[==========] Running 42 tests from 8 test suites.
[----------] Global test environment set-up.
[----------] 8 tests from CameraTest
[ RUN      ] CameraTest.DefaultConstructor
[       OK ] CameraTest.DefaultConstructor (1 ms)
[ RUN      ] CameraTest.ViewMatrixCalculation
[       OK ] CameraTest.ViewMatrixCalculation (0 ms)
...
[----------] 8 tests from CameraTest (15 ms total)
[==========] 42 tests from 8 test suites ran. (125 ms total)
[  PASSED  ] 42 tests.
```

## Best Practices

### Writing Unit Tests
1. **Test Single Responsibility**: Each test should verify one specific behavior
2. **Use Descriptive Names**: Test names should clearly indicate what is being tested
3. **Arrange-Act-Assert**: Structure tests with clear setup, execution, and verification phases
4. **Test Edge Cases**: Include boundary conditions and error scenarios
5. **Mock Dependencies**: Isolate the system under test from external dependencies

### Example Test Structure
```cpp
TEST_F(CameraTest, ViewMatrixCalculation)
{
    // Arrange
    glm::vec3 position(0.0f, 0.0f, 3.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    CameraProps props;
    props.position = position;
    Camera camera(props);
    
    // Act
    auto view_matrix = camera.GetViewMatrix();
    
    // Assert
    EXPECT_FALSE(view_matrix == glm::mat4(0.0f));
    
    // Verify transformation behavior
    glm::vec4 world_point(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 view_point = view_matrix * world_point;
    EXPECT_LT(view_point.z, 0.0f); // Point should be in front of camera
}
```

### Performance Testing
- **Benchmarking**: Include performance tests for critical paths
- **Realistic Workloads**: Test with representative data sizes
- **Platform Considerations**: Be aware of platform-specific performance characteristics
- **Regression Detection**: Set reasonable performance thresholds

### Integration Testing
- **End-to-End Workflows**: Test complete feature implementations
- **Subsystem Interactions**: Verify proper communication between components
- **Real-World Scenarios**: Simulate actual usage patterns
- **Error Propagation**: Test how errors are handled across system boundaries

## Continuous Integration

### Automated Testing
- **Pre-commit Hooks**: Run relevant tests before code commits
- **Pull Request Validation**: All tests must pass before merging
- **Nightly Builds**: Comprehensive test runs on multiple configurations
- **Performance Monitoring**: Track performance trends over time

### Test Coverage
- **Aim for High Coverage**: Target 80%+ code coverage for critical components
- **Quality over Quantity**: Focus on meaningful tests rather than coverage metrics
- **Document Exclusions**: Clearly document any code excluded from testing

## Debugging Failed Tests

### Common Issues
1. **OpenGL Context**: Many renderer tests require graphics context
2. **Resource Dependencies**: Tests may need asset files or mock resources
3. **Platform Differences**: Some tests may behave differently across platforms
4. **Timing Issues**: Performance tests may fail on slower systems

### Debugging Strategies
- **Isolate Failures**: Run individual test cases to identify specific issues
- **Enable Verbose Output**: Use `--gtest_verbose` for detailed test output
- **Debug Builds**: Use debug configurations for easier troubleshooting
- **Mock Systems**: Replace complex dependencies with simpler test doubles

## Contributing New Tests

### Guidelines
1. **Follow Naming Conventions**: Use descriptive, consistent test names
2. **Use Appropriate Fixtures**: Leverage existing test fixtures where possible
3. **Add Documentation**: Include comments explaining complex test logic
4. **Consider Performance**: Add performance tests for new critical features
5. **Update This Document**: Add new test categories to this documentation

### Test Review Checklist
- [ ] Tests follow established patterns and conventions
- [ ] All test cases have clear, descriptive names
- [ ] Tests are properly categorized (unit vs integration)
- [ ] Performance tests include reasonable thresholds
- [ ] Integration tests mock external dependencies appropriately
- [ ] Tests include both positive and negative test cases
- [ ] Code coverage is maintained or improved

## Future Enhancements

### Planned Improvements
- **Graphics Testing**: Integration with headless OpenGL for renderer testing
- **Asset Testing**: Automated testing of model and texture loading
- **Scripting Tests**: Testing of any future scripting system integration
- **Network Testing**: If networking features are added
- **Physics Testing**: When physics system is implemented

### Testing Tools Integration
- **Code Coverage**: Integration with coverage analysis tools
- **Static Analysis**: Automated code quality checks
- **Memory Testing**: Leak detection and memory usage analysis
- **Performance Profiling**: Integration with profiling tools for performance regression detection