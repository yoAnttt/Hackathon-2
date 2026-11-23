#pragma once

// Manages the visibility state of the interactive diagram nodes.
// Simplified with AI assistance (Google Gemini) to replace redundant Raylib wrapper methods.
class DiagramNode {
private:
    bool displayCircle;
    bool displaySubNodes;

public:
    DiagramNode();

    // Controls visibility of the main center circle
    bool ShowCircle() const;
    void SetCircle(bool circle);
    void ToggleCircle();

    // Controls visibility of the outer compartment circles and connecting lines
    bool ShowSubNodes() const;
    void SetSubNodes(bool subNodes);
    void ToggleSubNodes();
};