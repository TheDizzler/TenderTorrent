#include "../../pch.h"
#include "Attachment.h"

Attachment::~Attachment() {
}

void Attachment::setOffset(const Vector2& posOff) {
	positionOffset = posOff;
}

void Attachment::updatePosition(const Vector2& shipPosition) {
	attachmentLocation = shipPosition + positionOffset;
	setPosition(attachmentLocation);
}
