#pragma once

class DiagramNode {
private:
    bool displayCircle;
    bool displaySubNodes;

public:
    DiagramNode();

    bool ShowCircle() const;
    void SetCircle(bool circle);
    void ToggleCircle();

    bool ShowSubNodes() const;
    void SetSubNodes(bool subNodes);
    void ToggleSubNodes();
};