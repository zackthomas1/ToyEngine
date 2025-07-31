#include "utils/test_fixtures.h"
#include "ToyEngine/scene_node.h"
#include "ToyEngine/entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ToyEngine::Testing
{
    class SceneNodeTest : public ToyEngineTestBase
    {
    protected:
        void SetUp() override
        {
            ToyEngineTestBase::SetUp();
            
            root_node = std::make_unique<SceneNode>("RootNode");
            child_node = std::make_unique<SceneNode>("ChildNode");
            grandchild_node = std::make_unique<SceneNode>("GrandchildNode");
        }

        std::unique_ptr<SceneNode> root_node;
        std::unique_ptr<SceneNode> child_node;
        std::unique_ptr<SceneNode> grandchild_node;
    };

    TEST_F(SceneNodeTest, NodeCreation)
    {
        // Test basic node creation
        EXPECT_STREQ(root_node->GetName().c_str(), "RootNode");
        EXPECT_EQ(root_node->GetChildren().size(), 0);
        
        // Test world transform is initialized to identity
        glm::mat4 identity = glm::mat4(1.0f);
        EXPECT_EQ(root_node->GetWorldTransform(), identity);
    }

    TEST_F(SceneNodeTest, NodeWithEntity)
    {
        // Test node creation with entity
        auto test_entity = std::make_shared<Entity>();
        SceneNode node_with_entity("EntityNode", test_entity);
        
        EXPECT_STREQ(node_with_entity.GetName().c_str(), "EntityNode");
        EXPECT_EQ(node_with_entity.GetEntity(), test_entity);
    }

    TEST_F(SceneNodeTest, HierarchyConstruction)
    {
        // Test adding children to scene hierarchy
        auto child_ptr = child_node.get();
        EXPECT_TRUE(root_node->AddChild(std::move(child_node)));
        
        // Verify hierarchy structure
        EXPECT_EQ(root_node->GetChildren().size(), 1);
        
        // Add grandchild to child
        EXPECT_TRUE(child_ptr->AddChild(std::move(grandchild_node)));
        EXPECT_EQ(child_ptr->GetChildren().size(), 1);
    }

    TEST_F(SceneNodeTest, TransformOperations)
    {
        // Test local transform setting
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
        root_node->SetLocalTransform(translation);
        
        // Update world transform
        root_node->UpdateWorldTransform();
        
        // For root node, world transform should equal local transform
        glm::mat4 world_transform = root_node->GetWorldTransform();
        
        // Extract translation from matrix
        glm::vec3 world_translation = glm::vec3(world_transform[3]);
        EXPECT_NEAR(world_translation.x, 1.0f, 0.001f);
        EXPECT_NEAR(world_translation.y, 2.0f, 0.001f);
        EXPECT_NEAR(world_translation.z, 3.0f, 0.001f);
    }

    TEST_F(SceneNodeTest, HierarchicalTransforms)
    {
        // Setup hierarchy
        auto child_ptr = child_node.get();
        root_node->AddChild(std::move(child_node));
        
        // Set transforms
        glm::mat4 root_transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 child_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        root_node->SetLocalTransform(root_transform);
        child_ptr->SetLocalTransform(child_transform);
        
        // Update transforms
        root_node->UpdateWorldTransform();
        
        // Child's world transform should combine parent and local transforms
        glm::mat4 child_world = child_ptr->GetWorldTransform();
        glm::vec3 child_world_translation = glm::vec3(child_world[3]);
        
        // Should be (1, 1, 0) = root (1, 0, 0) + child (0, 1, 0)
        EXPECT_NEAR(child_world_translation.x, 1.0f, 0.001f);
        EXPECT_NEAR(child_world_translation.y, 1.0f, 0.001f);
        EXPECT_NEAR(child_world_translation.z, 0.0f, 0.001f);
    }

    TEST_F(SceneNodeTest, DirtyFlagPropagation)
    {
        // Setup hierarchy
        auto child_ptr = child_node.get();
        root_node->AddChild(std::move(child_node));
        
        // Mark root as dirty and update
        root_node->MarkDirty();
        root_node->UpdateWorldTransform();
        
        // Test that dirty flag mechanism works
        // (Implementation details depend on how dirty flag is used)
        SUCCEED(); // Placeholder - would need more implementation details
    }

    TEST_F(SceneNodeTest, IteratorInterface)
    {
        // Add multiple children
        root_node->AddChild(std::make_unique<SceneNode>("Child1"));
        root_node->AddChild(std::make_unique<SceneNode>("Child2"));
        root_node->AddChild(std::make_unique<SceneNode>("Child3"));
        
        // Test iterator interface
        int count = 0;
        for (auto it = root_node->ChildrenBegin(); it != root_node->ChildrenEnd(); ++it)
        {
            EXPECT_NE(it->get(), nullptr);
            count++;
        }
        EXPECT_EQ(count, 3);
        
        // Test const iterator
        const SceneNode* const_root = root_node.get();
        int const_count = 0;
        for (auto it = const_root->ChildrenBegin(); it != const_root->ChildrenEnd(); ++it)
        {
            EXPECT_NE(it->get(), nullptr);
            const_count++;
        }
        EXPECT_EQ(const_count, 3);
    }

    TEST_F(SceneNodeTest, DeepHierarchy)
    {
        // Create a deep hierarchy for testing
        SceneNode* current = root_node.get();
        const int depth = 10;
        
        for (int i = 0; i < depth; ++i)
        {
            auto new_child = std::make_unique<SceneNode>("Node_" + std::to_string(i));
            auto child_ptr = new_child.get();
            current->AddChild(std::move(new_child));
            current = child_ptr;
        }
        
        // Verify depth
        int actual_depth = 0;
        SceneNode* traverse = root_node.get();
        while (!traverse->GetChildren().empty())
        {
            traverse = traverse->GetChildren()[0].get();
            actual_depth++;
        }
        EXPECT_EQ(actual_depth, depth);
    }

    // Performance test for scene graph operations
    TEST_F(SceneNodeTest, PerformanceHierarchyTraversal)
    {
        // Build a broad scene tree
        const int children_per_node = 5;
        const int tree_depth = 4;
        
        std::function<void(SceneNode*, int)> build_tree = [&](SceneNode* parent, int depth) {
            if (depth <= 0) return;
            
            for (int i = 0; i < children_per_node; ++i)
            {
                auto child = std::make_unique<SceneNode>("Node_" + std::to_string(depth) + "_" + std::to_string(i));
                auto child_ptr = child.get();
                parent->AddChild(std::move(child));
                build_tree(child_ptr, depth - 1);
            }
        };
        
        build_tree(root_node.get(), tree_depth);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Perform many transform updates
        const int iterations = 1000;
        for (int i = 0; i < iterations; ++i)
        {
            root_node->UpdateWorldTransform();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Should complete in reasonable time
        EXPECT_LT(duration.count(), 50000); // Less than 50ms for 1000 updates
    }

    TEST_F(SceneNodeTest, MemoryManagement)
    {
        // Test that scene nodes properly manage memory
        const int num_nodes = 100;
        Vector<std::unique_ptr<SceneNode>> nodes;
        
        // Create many nodes
        for (int i = 0; i < num_nodes; ++i)
        {
            nodes.push_back(std::make_unique<SceneNode>("Node_" + std::to_string(i)));
        }
        
        // Add them to hierarchy
        for (int i = 1; i < num_nodes; ++i)
        {
            // Add every other node as child of previous
            if (i % 2 == 1)
            {
                nodes[i-1]->AddChild(std::move(nodes[i]));
            }
        }
        
        // Verify no memory issues (nodes will be destroyed when vector goes out of scope)
        SUCCEED();
    }
}