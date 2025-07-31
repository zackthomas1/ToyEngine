#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ToyEngine/core.h"

// Common test utilities and fixtures
namespace ToyEngine::Testing
{
    /// <summary>
    /// Base test fixture class that provides common setup and teardown functionality
    /// for ToyEngine tests. Inherits from Google Test's Test class.
    /// </summary>
    class ToyEngineTestBase : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Common setup for all tests
        }

        void TearDown() override
        {
            // Common cleanup for all tests
        }
    };

    /// <summary>
    /// Test fixture for renderer-related tests that need OpenGL context simulation.
    /// Provides mock objects and utilities for testing rendering components.
    /// </summary>
    class RendererTestFixture : public ToyEngineTestBase
    {
    protected:
        void SetUp() override
        {
            ToyEngineTestBase::SetUp();
            // Renderer-specific setup
        }

        void TearDown() override
        {
            // Renderer-specific cleanup
            ToyEngineTestBase::TearDown();
        }
    };

    /// <summary>
    /// Test fixture for application-level tests that need window and event system simulation.
    /// </summary>
    class ApplicationTestFixture : public ToyEngineTestBase
    {
    protected:
        void SetUp() override
        {
            ToyEngineTestBase::SetUp();
            // Application-specific setup
        }

        void TearDown() override
        {
            // Application-specific cleanup
            ToyEngineTestBase::TearDown();
        }
    };

    /// <summary>
    /// Test fixture for model and asset loading tests.
    /// </summary>
    class ModelTestFixture : public ToyEngineTestBase
    {
    protected:
        void SetUp() override
        {
            ToyEngineTestBase::SetUp();
            // Model loading specific setup
        }

        void TearDown() override
        {
            // Model loading specific cleanup
            ToyEngineTestBase::TearDown();
        }
    };
}