#pragma once

namespace ToyEngine
{
  // @brief The TimeStep class provides an interface for managing time steps in the application.
  class TimeStep
  {
  public:
    /// @brief Virtual destructor for the TimeStep class. 
    /// Ensures proper cleanup of derived classes.
    virtual ~TimeStep() = default;

    /// @brief Initializes the time step service. 
    /// This method should be called once at the start of the application.
    virtual void Init() = 0;
    
    /// @brief Increment time step.
    /// WARNING: Only call this method from the Application::Run().
    /// The time step should only be modified in the main game loop.
    virtual void Update() = 0;

    /// @brief Retrieves the time delta (elapsed time) since the last frame in seconds.
    /// @return The time delta in seconds.
    virtual float GetTimeDelta() const = 0;

    /// @brief Retrieves the current time in seconds since the application started.
    /// @return The current time in seconds.
    virtual float GetTimeCurrent() const = 0;
  protected:
    /// @brief Default constructor for the TimeStep class.
    /// Protected to prevent direct instantiation.
    TimeStep() = default;
  };

  /// @brief A NullTimeStep class that implements the TimeStep interface but does not perform any actual time step calculations.
  class NullTimeStep : public TimeStep
  {
  public:

    NullTimeStep() = default;
    ~NullTimeStep() = default;

    virtual void Init() override
    {
      TY_CORE_WARN( "Null service provider - Time step service disabled." );
    }
    virtual void Update() override {}
    virtual float GetTimeDelta() const override { return 0.0f; }
    virtual float GetTimeCurrent() const override { return 0.0f; }
  };

  // @brief Concrete implementation of the TimeStep class using GLFW for time management.
  class TimeStepGLFW : public TimeStep
  {
  public:
    /// @brief Constructs a TimeStepGLFW instance and initializes internal time variables.
    TimeStepGLFW();

    /// @brief Destructor for the TimeStepGLFW class. 
    /// Ensures proper cleanup of resources.
    ~TimeStepGLFW() = default; 
    
    /// @brief Initializes the time step service. 
    /// This method should be called once at the start of the application.
    virtual void Init() override;

    /// @brief Increment time step.
    /// WARNING: Only call this method from the Application::Run().
    /// The time step should only be modified in the main game loop.
    virtual void Update() override;

    /// @brief Retrieves the time delta (elapsed time) since the last frame in seconds.
    /// @return The time delta in seconds.
    virtual float GetTimeDelta() const override;
    
    /// @brief Retrieves the current time in seconds since the application started.
    /// @return The current time in seconds.
    virtual float GetTimeCurrent() const override;
  private:
    /* Internal time variables */
    float current_time_;
    /* Time of the last frame */
    float last_frame_time_;
    /* Time step (delta time) */
    float time_step_;
  };
}