#include "Tempfun.h"

// Constructor initializes both visibility states to hidden (false).
// Initial layout and debugging assisted by AI.
DiagramNode::DiagramNode() {
    displayCircle = false;
    displaySubNodes = false;
}

bool DiagramNode::ShowCircle() const {
    return displayCircle;
}

void DiagramNode::SetCircle(bool circle) {
    displayCircle = circle;
}

void DiagramNode::ToggleCircle() {
    displayCircle = !displayCircle;
}

bool DiagramNode::ShowSubNodes() const {
    return displaySubNodes;
}

void DiagramNode::SetSubNodes(bool subNodes) {
    displaySubNodes = subNodes;
}

void DiagramNode::ToggleSubNodes() {
    displaySubNodes = !displaySubNodes;
}