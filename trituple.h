#pragma once
template <typename A, typename B, typename C>
struct trituple{
  A first;
  B second;
  C third;
	trituple() = default;
  trituple(const A& first, const B& second, const C& third): first{first}, second{second}, third{third}{}
};