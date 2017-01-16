#include <project/questlog.h>

QuestLog::QuestLog()
{
	this->infoText = new HudText();
	this->addChild(infoText);
	this->infoText->loadFont("assets/arial.ttf");
	this->infoText->setMultiLine(true);
	this->infoText->color = BLACK;

	surrounding = new Mesh();
	surrounding->setDrawMode(Mesh::drawModeSettings::lines);
	surrounding->setLineThickness(5);

	Texture* emptyTexture = new Texture();
	Sprite* sprite = new Sprite();
	sprite->setDynamics(surrounding, emptyTexture);
	this->addSprite(sprite);
	this->color = BLACK;

	padding = 5;
}


QuestLog::~QuestLog()
{
	removeChild(infoText);
	delete infoText;
}

void QuestLog::addQuest(std::string questName, std::string questInfo)
{
	quests[questName] = questInfo;

	reformatText();
}

void QuestLog::removeQuest(std::string questName)
{
	std::map<std::string, std::string>::iterator it;
	it = quests.find(questName);
	if (it != quests.end())
	{
		quests.erase(it);
	}

	reformatText();
}

void QuestLog::reformatText()
{
	std::string formattedText = "Quests\n-----------------\n";
	std::map<std::string, std::string>::iterator it = quests.begin();
	for (it = quests.begin(); it != quests.end(); it++) {
		formattedText += "Quest: ";
		formattedText += it->first;
		formattedText += "\n";
		formattedText += "Info:\n";
		formattedText += it->second;
		formattedText += "\n-----------------\n";
	}

	infoText->setText(formattedText);

	std::vector<Vector2> vertices;
	Vector2 halveSize = Vector2(infoText->getWidth()/2.0f, infoText->getHeight()/2.0f);
	halveSize += padding;
	vertices.push_back(Vector2(-halveSize.x, -halveSize.y));
	vertices.push_back(Vector2(halveSize.x, -halveSize.y));

	vertices.push_back(Vector2(halveSize.x, -halveSize.y));
	vertices.push_back(Vector2(halveSize.x, halveSize.y));

	vertices.push_back(Vector2(halveSize.x, halveSize.y));
	vertices.push_back(Vector2(-halveSize.x, halveSize.y));

	vertices.push_back(Vector2(-halveSize.x, halveSize.y));
	vertices.push_back(Vector2(-halveSize.x, -halveSize.y));

	surrounding->setFromVertices(vertices);

}
