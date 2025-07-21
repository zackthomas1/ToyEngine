# Render Command Queue

This document describes the render command queue system implemented for the ToyEngine rendering system.

## Overview

The render command queue decouples command recording from command execution, replacing the previous immediate-mode rendering with a more flexible queued approach.

## Architecture

### IRenderCommand Interface
All render commands implement the `IRenderCommand` interface with a single `Execute()` method.

### Command Types
- **DrawMeshCommand**: Renders a model with a world transform
- **ClearBackgroundCommand**: Clears the background with a specified color
- **SetPolygonModeCommand**: Sets polygon rendering mode (wireframe, filled, etc.)

### RenderCommandQueue
Manages a collection of render commands with methods to:
- `Enqueue()`: Add commands to the queue
- `ExecuteAll()`: Execute all queued commands and clear the queue
- `Clear()`: Clear commands without executing
- `Reserve()`: Pre-allocate memory to avoid reallocations

## Integration with Renderer

The `Renderer` class now uses the command queue:

1. **BeginScene()**: Clears the queue and enqueues a clear background command
2. **Submit()**: Enqueues draw commands instead of immediate rendering
3. **EndScene()**: Executes all queued commands

## Benefits

- **Decoupled execution**: Commands can be recorded on one thread and executed on another
- **Batching potential**: Commands can be sorted and batched for optimization
- **Better debugging**: Command queue can be inspected and commands can be replayed
- **Flexibility**: New command types can be easily added

## Usage Example

```cpp
// The old immediate-mode approach:
Renderer::BeginScene(camera, lights);
Renderer::Submit(model, transform);  // Immediate rendering
Renderer::EndScene();

// The new queued approach (same API, different implementation):
Renderer::BeginScene(camera, lights);  // Enqueues clear command
Renderer::Submit(model, transform);    // Enqueues draw command
Renderer::SetPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Enqueues polygon mode command
Renderer::EndScene();                   // Executes all commands
```

## Future Enhancements

- Command sorting for optimization (opaque objects first, transparent last)
- Command batching for similar draw calls
- Multi-threaded command recording
- Command buffer reuse to reduce allocations
- Debug visualization of command queue contents