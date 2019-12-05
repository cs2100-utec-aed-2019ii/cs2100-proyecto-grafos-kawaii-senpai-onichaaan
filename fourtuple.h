template <typename A, typename B, typename C, typename D>
struct fourtuple{
  A first;
  B second;
  C third;
	D fourth;
	fourtuple() = default;
  fourtuple(const A& first, const B& second, const C& third, const D& fourth): first{first}, second{second}, third{third}, fourth{fourth}{}
};