# Contributing to ToyEngine

We welcome contributions to ToyEngine! This guide will help you get started with contributing to the project.

## How to Contribute

### Types of Contributions

- **Bug Reports**: Report issues and bugs you encounter
- **Feature Requests**: Suggest new features or improvements
- **Code Contributions**: Implement new features, fix bugs, or improve existing code
- **Documentation**: Improve documentation, tutorials, and examples
- **Testing**: Add or improve unit tests and integration tests
- **Performance**: Optimize code for better performance

### Before You Start

1. **Read the Documentation**: Familiarize yourself with the project structure and goals
2. **Check Existing Issues**: Look for existing issues or discussions related to your contribution
3. **Join the Community**: Participate in discussions and ask questions
4. **Understand the Codebase**: Explore the code to understand the architecture and patterns

## Getting Started

### 1. Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/yourusername/ToyEngine.git
   cd ToyEngine
   ```

### 2. Set Up Development Environment

1. Follow the [Quick Start Guide](quick-start.md) to set up the project
2. Ensure you can build and run the engine successfully
3. Run any existing tests to make sure everything works

### 3. Create a Branch

Create a new branch for your contribution:
```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/your-bug-fix
```

## Development Guidelines

### Code Style

Follow these coding standards:

#### C++ Style
- Use **PascalCase** for class names: `MyClass`
- Use **camelCase** for function names: `myFunction()`
- Use **snake_case** for variable names: `my_variable`
- Use **SCREAMING_SNAKE_CASE** for constants: `MAX_BUFFER_SIZE`
- Use **m_** prefix for member variables: `m_position`
- Use **s_** prefix for static variables: `s_instance`

#### Example:
```cpp
class RenderManager
{
public:
    RenderManager();
    ~RenderManager();
    
    void initialize();
    void renderFrame();
    
    static RenderManager& getInstance();
    
private:
    bool m_isInitialized = false;
    uint32_t m_frameCount = 0;
    
    static RenderManager* s_instance;
    
    void setupBuffers();
};
```

#### Formatting
- Use 4 spaces for indentation (no tabs)
- Place opening braces on the same line
- Use spaces around operators: `a + b`, not `a+b`
- Keep lines under 120 characters when possible

### Documentation

- Document all public APIs with comments
- Use Doxygen-style comments for functions and classes
- Keep comments up to date with code changes
- Write clear commit messages

#### Example:
```cpp
/**
 * @brief Renders a 3D model with the specified shader
 * @param shader The shader to use for rendering
 * @param model The model to render
 * @param transform The transformation matrix to apply
 * @return True if rendering was successful, false otherwise
 */
bool renderModel(const Ref<Shader>& shader, 
                 const Ref<Model>& model, 
                 const glm::mat4& transform);
```

### Testing

- Write unit tests for new functionality
- Ensure existing tests still pass
- Test on different platforms when possible
- Include performance tests for critical paths

### Performance

- Profile performance-critical code
- Avoid unnecessary allocations in hot paths
- Use appropriate data structures for the task
- Consider cache-friendly memory layouts

## Submitting Changes

### 1. Test Your Changes

Before submitting:
- Build the project in both Debug and Release configurations
- Run all existing tests
- Test your changes manually
- Check for memory leaks and performance regressions

### 2. Commit Your Changes

Write clear, descriptive commit messages:
```bash
git commit -m "Add shadow mapping support for directional lights

- Implement shadow map generation
- Add shadow sampling in fragment shader
- Update light data structure with shadow info
- Add shadow mapping example to editor

Fixes #123"
```

### 3. Push to Your Fork

```bash
git push origin feature/your-feature-name
```

### 4. Create a Pull Request

1. Go to the GitHub repository
2. Click "New Pull Request"
3. Select your branch and provide a detailed description
4. Reference any related issues

### Pull Request Guidelines

#### Description Template
```markdown
## Description
Brief description of the changes made.

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Testing
- [ ] Unit tests pass
- [ ] Integration tests pass
- [ ] Manual testing completed
- [ ] Performance testing completed (if applicable)

## Checklist
- [ ] Code follows the project's style guidelines
- [ ] Self-review of code completed
- [ ] Code is commented, particularly in hard-to-understand areas
- [ ] Documentation has been updated
- [ ] Changes generate no new warnings
- [ ] Tests have been added/updated as needed
```

## Code Review Process

### What to Expect

1. **Automated Checks**: CI/CD pipeline will run tests and checks
2. **Code Review**: Maintainers will review your code for:
   - Code quality and style
   - Functionality and correctness
   - Performance implications
   - Documentation completeness
3. **Feedback**: You may receive feedback and requests for changes
4. **Iteration**: Work with reviewers to address feedback
5. **Merge**: Once approved, your changes will be merged

### Review Criteria

- **Functionality**: Does the code work as intended?
- **Quality**: Is the code well-structured and maintainable?
- **Performance**: Are there any performance concerns?
- **Testing**: Are there adequate tests?
- **Documentation**: Is the code properly documented?
- **Style**: Does the code follow project conventions?

## Issue Guidelines

### Reporting Bugs

When reporting a bug, include:

1. **Clear Title**: Descriptive title summarizing the issue
2. **Environment**: OS, compiler version, graphics card, etc.
3. **Steps to Reproduce**: Clear steps to reproduce the issue
4. **Expected Behavior**: What should happen
5. **Actual Behavior**: What actually happens
6. **Screenshots/Logs**: Visual evidence or error logs
7. **Minimal Example**: Smallest code example that reproduces the issue

### Feature Requests

When requesting a feature:

1. **Clear Description**: What feature do you want?
2. **Use Case**: Why is this feature needed?
3. **Alternatives**: What alternatives have you considered?
4. **Implementation Ideas**: Any thoughts on implementation?

## Community Guidelines

### Code of Conduct

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Provide constructive feedback
- Focus on what is best for the community
- Show empathy towards other community members

### Communication

- Use GitHub Issues for bug reports and feature requests
- Use GitHub Discussions for general questions and ideas
- Be patient with response times
- Search existing issues before creating new ones

## Development Setup

### Required Tools

- **Git**: Version control
- **Visual Studio 2022**: C++ development environment
- **CMake**: Build system (planned)
- **Doxygen**: Documentation generation (planned)

### Recommended Tools

- **Visual Studio Code**: Alternative editor with C++ extensions
- **Git GUI**: GitKraken, SourceTree, or GitHub Desktop
- **Profiler**: Intel VTune, Visual Studio Profiler
- **Debugger**: Visual Studio Debugger, GDB

## Advanced Topics

### Performance Optimization

- Use profiling tools to identify bottlenecks
- Optimize hot paths first
- Consider memory layout and cache performance
- Use appropriate algorithms and data structures

### Graphics Programming

- Understand OpenGL concepts and best practices
- Test on different graphics hardware
- Consider cross-platform compatibility
- Stay updated with graphics API changes

### Engine Architecture

- Understand the modular design principles
- Maintain clean interfaces between systems
- Consider extensibility and maintainability
- Document architectural decisions

## Getting Help

### Resources

- **Documentation**: Read the existing documentation
- **Code Examples**: Look at existing code for patterns
- **GitHub Issues**: Search for similar issues or ask questions
- **Community**: Join discussions with other contributors

### Common Questions

**Q: How do I set up the development environment?**
A: Follow the [Quick Start Guide](quick-start.md) for detailed setup instructions.

**Q: What should I work on as a first contribution?**
A: Look for issues labeled "good first issue" or "help wanted" on GitHub.

**Q: How do I run the tests?**
A: Build the project and run the test executables in the output directory.

**Q: Where can I find coding examples?**
A: Check the Editor application and look at existing engine code.

## Recognition

Contributors will be recognized in:
- Project README contributors section
- Release notes for significant contributions
- Project documentation

Thank you for contributing to ToyEngine! Your contributions help make this project better for everyone.