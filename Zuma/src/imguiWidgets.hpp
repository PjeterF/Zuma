#pragma once
#include "zumalogic/SnakeManager.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <math.h>

void textureLoader(std::vector<Texture*>* allTextures, std::vector<Texture*>* segmentTextures, GameLevel* level)
{
	static char filepath[100] = "";
	
	ImVec2 windowSize = ImGui::GetWindowSize();
	int imageSize = 50;
	int maxImagesPerRow = windowSize.x / imageSize - 3;
	static int succesfulLoad = true;
	if (maxImagesPerRow <= 0)
	{
		maxImagesPerRow = 1;
	}
	ImGui::Text("Texture filepath");

	if (ImGui::Button("Load"))
	{
		Texture* newTexture = new Texture(filepath);
		if (newTexture->getId() != 0)
		{
			succesfulLoad = true;
			allTextures->push_back(newTexture);
			std::fill_n(filepath, 100, 0);
		}
		else
		{
			succesfulLoad = false;
			delete newTexture;
		}

	}
	ImGui::SameLine();
	ImGui::InputText(" ", filepath, 100);
	if (succesfulLoad)
	{
		ImGui::Dummy(ImVec2(ImVec2(0, 0)));
	}
	else
	{
		ImGui::Text("Failed to load texure");
	}

	std::vector<std::string> popupIDs;
	for (int i = 0; i < allTextures->size(); i++)
	{
		popupIDs.push_back("");
		popupIDs[i].append(1, i);
		Texture* currentTexture = allTextures->at(i);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if(ImGui::ImageButton((ImTextureID)currentTexture->getId(), ImVec2(imageSize, imageSize)))
		{
			ImGui::OpenPopup(popupIDs[i].c_str());
		}
		if (ImGui::BeginPopup(popupIDs[i].c_str()))
		{
			if (ImGui::Button("Add snake segment with this texture"))
			{
				std::vector<Texture*>::iterator it = segmentTextures->begin();
				bool found = false;
				while (it != segmentTextures->end())
				{
					if ((*it)->getId() == currentTexture->getId())
					{
						found = true;
						break;
					}
					it++;
				}
				if(!found)
					segmentTextures->push_back(currentTexture);
			}
			if (ImGui::Button("Set as shooter texture"))
			{
				level->shooter->setTexture(currentTexture);
			}
			if (ImGui::Button("Set as background"))
			{
				level->setBackground(currentTexture);
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::BeginTooltip();
			ImGui::Text("ID:%d", currentTexture->getId());
			ImGui::EndTooltip();
		}
		if (i == 0 || i % maxImagesPerRow != 0)
		{
			ImGui::SameLine();
		}
	}
}

void snakeEdit(CubicBezierSpline* route, SnakeManager* manager, std::vector<Texture*>* segmentTextures)
{
	static int numberOfSegments = 1;
	static float segmentSize = 10;
	static float velocity = 2;
	static float distanceBetweenSegments = 60;
	ImGui::PushItemWidth(50);
	ImGui::InputInt("Number of segments", &numberOfSegments, 0);
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Segment size", &segmentSize, 0, 50);
	ImGui::SliderFloat("Velocity", &velocity, 0.001, 10);
	ImGui::SliderFloat("Segment spacing", &distanceBetweenSegments, 0.001, 200);
	if (ImGui::Button("Commit changes and reset"))
	{
		Snake::setDeistanceBetweenSegments(distanceBetweenSegments);
		manager->getShooter()->setProjectileSize(segmentSize);
		manager->getShooter()->reroll();
		manager->replaceInitialSnake(new Snake(segmentSize, velocity, route, numberOfSegments, segmentTextures));
	}

	ImVec2 windowSize = ImGui::GetWindowSize();
	int imageSize = 50;
	int maxImagesPerRow = windowSize.x / imageSize - 3;
	if (maxImagesPerRow <= 0)
	{
		maxImagesPerRow = 1;
	}
	std::vector<std::string> popupIDs;
	for (int i = 0; i < segmentTextures->size(); i++)
	{
		popupIDs.push_back("");
		popupIDs[i].append(1, i);
		Texture* currentTexture = segmentTextures->at(i);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((ImTextureID)currentTexture->getId(), ImVec2(imageSize, imageSize)))
		{
			ImGui::OpenPopup(popupIDs[i].c_str());
		}
		if (ImGui::BeginPopup(popupIDs[i].c_str()))
		{
			if (ImGui::Button("Remove form snake"))
			{
				segmentTextures->erase(segmentTextures->begin() + i);
				Snake::setDeistanceBetweenSegments(distanceBetweenSegments);
				manager->getShooter()->setProjectileSize(segmentSize);
				manager->getShooter()->reroll();
				manager->replaceInitialSnake(new Snake(segmentSize, velocity, route, numberOfSegments, segmentTextures));
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
		if (i == 0 || i % maxImagesPerRow != 0)
		{
			ImGui::SameLine();
		}
	}
}

void shooterEdit(SnakeManager* manager, GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	static float currentOffset = manager->getShooter()->getScale().x;
	static float nextOffsetX = 0;
	static float nextOffsetY = 0;
	static float shooterSize = manager->getShooter()->getScale().x;
	static float posX = manager->getShooter()->getPosition().x;
	static float posY = manager->getShooter()->getPosition().y;
	ImGui::Text("Current offset"); ImGui::SameLine(150); ImGui::SliderFloat(" ", &currentOffset, 0, manager->getShooter()->getScale().x);
	ImGui::Text("Next offset X"); ImGui::SameLine(150); ImGui::SliderFloat("  ", &nextOffsetX, 0, manager->getShooter()->getScale().x);
	ImGui::Text("Next offset Y"); ImGui::SameLine(150); ImGui::SliderFloat("   ", &nextOffsetY, 0, manager->getShooter()->getScale().x);
	ImGui::Text("Shooter size"); ImGui::SameLine(150); ImGui::SliderFloat("    ", &shooterSize, 0, 300);
	ImGui::Text("Pos X"); ImGui::SameLine(150); ImGui::SliderFloat("     ", &posX, 0, width);
	ImGui::Text("Pos Y"); ImGui::SameLine(150); ImGui::SliderFloat("      ", &posY, 0, height);
	if (currentOffset > shooterSize)
	{
		currentOffset = shooterSize;
	}
	manager->getShooter()->setScale(shooterSize);
	manager->getShooter()->setCurrentOffset(currentOffset);
	manager->getShooter()->setNextOffset(glm::vec2(nextOffsetX, nextOffsetY));
	manager->getShooter()->setPosition(posX, posY);
}

void levelEdit(GameLevel* level)
{
	
}