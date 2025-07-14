# Troubleshooting Guide

Common issues and solutions when working with ToyEngine.

## Build Issues

### "premake5.exe not found"
**Problem**: Cannot find the premake5 executable.

**Solution**: 
1. Check that `vendor/premake/premake5.exe` exists
2. If missing, re-clone the repository or download premake5 manually
3. Make sure you've initialized git submodules:
   ```bash
   git submodule update --init --recursive
   ```

### "Cannot open include file: 'pch.h'"
**Problem**: Precompiled header not found during compilation.

**Solution**:
1. Ensure you're building with Visual Studio 2022
2. Clean and rebuild the solution
3. Check that `ToyEngine/src/pch.h` exists
4. Verify that the precompiled header is enabled in project settings

### "Unresolved external symbol" errors
**Problem**: Linker cannot find function implementations.

**Solution**:
1. Check that all dependencies are properly linked
2. Ensure static libraries are built before the main project
3. Verify that the correct platform (x64) is selected
4. Make sure you're linking against the correct configuration (Debug/Release)

### "The system cannot find the file specified"
**Problem**: Cannot find asset files or libraries at runtime.

**Solution**:
1. Check that the working directory is set to the project root
2. Verify that assets exist in the `assets/` directory
3. Ensure the Editor project is set as the startup project
4. Check that all DLLs are in the output directory

## Runtime Issues

### Black Screen on Startup
**Problem**: Application runs but shows only a black screen.

**Causes & Solutions**:
1. **Graphics driver issue**: Update your graphics drivers
2. **OpenGL version**: Ensure your GPU supports OpenGL 4.3+
3. **Shader compilation**: Check console for shader compilation errors
4. **Asset loading**: Verify that models and textures are loading correctly

### Application Crashes on Startup
**Problem**: Application crashes immediately or shortly after startup.

**Debugging Steps**:
1. Run in Debug mode to get detailed error information
2. Check the console output for error messages
3. Use the Visual Studio debugger to find the crash location
4. Verify that all required DLLs are available

**Common Causes**:
- Missing or corrupted assets
- Graphics driver incompatibility
- Insufficient system requirements
- Corrupted project files

### Poor Performance/Low FPS
**Problem**: Application runs slowly or has inconsistent frame rates.

**Solutions**:
1. **Switch to Release mode**: Debug builds are significantly slower
2. **Update graphics drivers**: Ensure you have the latest drivers
3. **Check system requirements**: Verify your hardware meets minimum specs
4. **Reduce scene complexity**: Try with fewer models or lower resolution textures
5. **Monitor GPU usage**: Use tools like MSI Afterburner to check GPU utilization

### Input Not Working
**Problem**: Keyboard or mouse input doesn't respond.

**Solutions**:
1. Ensure the application window has focus
2. Check that the input polling system is initialized
3. Verify that event callbacks are properly registered
4. Test with a clean build to rule out configuration issues

## Graphics Issues

### Textures Not Loading
**Problem**: Models appear without textures or with incorrect textures.

**Solutions**:
1. Check that texture files exist in the correct directory
2. Verify supported formats (PNG, JPG, TGA)
3. Check console for texture loading error messages
4. Ensure texture paths in models are correct

### Lighting Issues
**Problem**: Lighting appears incorrect or objects are too dark/bright.

**Solutions**:
1. Check that normal vectors are correctly calculated
2. Verify that light positions and colors are set correctly
3. Ensure shaders are compiling without errors
4. Check that material properties are properly set

### Shader Compilation Errors
**Problem**: Shaders fail to compile with error messages.

**Solutions**:
1. Check shader syntax for OpenGL version compatibility
2. Verify that all required uniforms are defined
3. Check for missing #version directives
4. Ensure attribute locations are correctly specified

## Platform-Specific Issues

### Windows-Specific

#### "VCRUNTIME140.dll not found"
**Solution**: Install Visual C++ Redistributable for Visual Studio 2022

#### "OpenGL32.dll not found"
**Solution**: Update graphics drivers or install OpenGL libraries

#### Antivirus Software Interference
**Solution**: Add ToyEngine directory to antivirus exclusions

### Development Environment Issues

#### Visual Studio 2022 Not Detecting Project
**Solution**:
1. Ensure you're opening the `.sln` file, not individual project files
2. Check that all project files were generated correctly by premake5
3. Try regenerating project files with `scripts/Win-GenProjects.bat`

#### IntelliSense Errors
**Solution**:
1. Rebuild the solution to update IntelliSense database
2. Clear Visual Studio cache (delete `.vs` folder)
3. Check that include paths are correctly set

## Asset-Related Issues

### Model Loading Failures
**Problem**: 3D models fail to load or appear corrupted.

**Solutions**:
1. **Check file format**: Ensure models are in supported formats (OBJ, FBX)
2. **Verify file paths**: Check that model files exist at specified locations
3. **Check model structure**: Ensure models have valid geometry and materials
4. **Test with simple models**: Try loading a basic cube or sphere first

### Texture Loading Issues
**Problem**: Textures don't load or appear corrupted.

**Solutions**:
1. **Check image format**: Ensure textures are in supported formats
2. **Verify dimensions**: Check that texture dimensions are power of 2
3. **Check file size**: Ensure texture files aren't corrupted
4. **Test with simple textures**: Try loading a basic checkered pattern

## Performance Issues

### Memory Leaks
**Problem**: Memory usage increases over time.

**Debugging**:
1. Use Visual Studio Diagnostic Tools to monitor memory usage
2. Check for unreleased resources (textures, buffers, etc.)
3. Verify that destructors are being called properly
4. Use static analysis tools to detect potential leaks

### CPU Performance Issues
**Problem**: High CPU usage or long frame times.

**Solutions**:
1. **Profile the application**: Use Visual Studio Profiler to identify bottlenecks
2. **Optimize hot paths**: Focus on functions called frequently
3. **Reduce draw calls**: Batch similar objects together
4. **Implement frustum culling**: Don't render objects outside the view

### GPU Performance Issues
**Problem**: GPU-bound performance problems.

**Solutions**:
1. **Reduce polygon count**: Use simpler models or LOD systems
2. **Optimize shaders**: Reduce shader complexity
3. **Batch draw calls**: Minimize state changes
4. **Use appropriate texture sizes**: Don't use oversized textures

## Debugging Tips

### Using Visual Studio Debugger
1. Set breakpoints in problematic code
2. Use the Immediate Window to examine variables
3. Step through code line by line
4. Check the Call Stack window for function call hierarchy

### Console Output
1. Enable detailed logging in Debug builds
2. Check for warning and error messages
3. Use custom log messages to trace execution
4. Monitor OpenGL error states

### Graphics Debugging
1. Use graphics debugging tools (RenderDoc, Intel GPA)
2. Check OpenGL state before and after operations
3. Verify vertex and index buffer contents
4. Inspect shader uniforms and attributes

## Getting Help

### Before Asking for Help
1. Search existing GitHub issues for similar problems
2. Try the solutions in this troubleshooting guide
3. Test with a minimal example to isolate the issue
4. Gather relevant information (OS, hardware, error messages)

### Where to Get Help
- **GitHub Issues**: Report bugs and get help from maintainers
- **GitHub Discussions**: Ask questions and share ideas
- **Community Forums**: Connect with other ToyEngine users
- **Documentation**: Check the wiki for detailed information

### Information to Include
When asking for help, provide:
- Operating system and version
- Visual Studio version
- Graphics card and driver version
- Exact error messages
- Steps to reproduce the issue
- Screenshots or videos if applicable
- Minimal code example that demonstrates the problem

## Common Error Messages

### "Failed to create OpenGL context"
**Cause**: Graphics driver or hardware doesn't support required OpenGL version.
**Solution**: Update graphics drivers or use compatible hardware.

### "Shader compilation failed"
**Cause**: Syntax errors or incompatible shader code.
**Solution**: Check shader source code and OpenGL version compatibility.

### "Failed to load texture"
**Cause**: Texture file not found or in unsupported format.
**Solution**: Verify file path and format, check file permissions.

### "Access violation" or "Segmentation fault"
**Cause**: Memory access error, often due to null pointers or buffer overruns.
**Solution**: Use debugger to locate exact cause, check pointer validity.

### "Stack overflow"
**Cause**: Infinite recursion or very deep function calls.
**Solution**: Check for recursive loops, optimize call stack depth.

## Prevention Tips

### Good Development Practices
1. **Regular testing**: Test changes frequently during development
2. **Version control**: Use Git to track changes and revert if needed
3. **Code reviews**: Have others review your code for potential issues
4. **Documentation**: Keep code well-documented for easier debugging

### System Maintenance
1. **Keep drivers updated**: Regularly update graphics and system drivers
2. **Monitor system resources**: Watch memory and CPU usage
3. **Clean builds**: Occasionally clean and rebuild the entire project
4. **Backup important work**: Keep backups of important project files

### Performance Monitoring
1. **Profile regularly**: Use profiling tools to catch performance issues early
2. **Monitor frame times**: Keep track of rendering performance
3. **Test on different hardware**: Ensure compatibility across systems
4. **Optimize incrementally**: Make small optimizations rather than large changes

Remember: Most issues have been encountered before, so don't hesitate to search for solutions or ask for help!