#include "csp.h"

#ifdef INLINE_CSP
//#warning "INFO - inlining CSP methods"
#define INLINE inline
#else   
//#warning "INFO - NOT inlining CSP methods"
#define INLINE 
#endif


////////////////////////////////////////////////////////////
//CSP constructor
template <typename T>
CSP<T>::CSP(T& cg) :
	arc_consistency(),
	cg(cg),
	solution_counter(0),
	recursive_call_counter(0),
	iteration_counter(0)
{
}
////////////////////////////////////////////////////////////
//CSP solver, brute force - no forward checking
template <typename T>
bool CSP<T>::SolveDFS(unsigned level) {
	++recursive_call_counter;
	if (cg.AllVariablesAssigned())
	{
		++solution_counter;
		return true;
	}
	Variable* var_to_assign = MinRemVal();
	std::set<Value> save_domain = var_to_assign->GetDomain();
	for (int i = 0; i < var_to_assign->SizeDomain(); ++i)
	{
		++iteration_counter;
		var_to_assign->Assign();
		bool goodToGo = AssignmentIsConsistent(var_to_assign);
		bool result = false;
		if (goodToGo)
			result = SolveDFS(level + 1);
		if (result)
			return true;
		var_to_assign->RemoveValue(var_to_assign->GetValue());
		var_to_assign->UnAssign();
		--i;
		if (var_to_assign->IsImpossible())
		{
			var_to_assign->SetDomain(save_domain);
			return false;
		}
	}
	return false;
}


////////////////////////////////////////////////////////////
//CSP solver, uses forward checking
template <typename T>
bool CSP<T>::SolveFC(unsigned level) {
	++recursive_call_counter;
	if (cg.AllVariablesAssigned())
	{
		++solution_counter;
		return true;
	}
	//choose a variable by MRV
	Variable* var_to_assign = MinRemVal();
	auto to_save = SaveState(var_to_assign);
	for (int i = 0; i < var_to_assign->SizeDomain(); ++i)
	{
		++iteration_counter;
		var_to_assign->Assign();
		bool goodToGo = AssignmentIsConsistent(var_to_assign);
		bool fc = ForwardChecking(var_to_assign);
		bool result = false;
		if (goodToGo && fc)
			result = SolveFC(level + 1);
		if (result)
			return true;
		LoadState(to_save);
		var_to_assign->RemoveValue(var_to_assign->GetValue());
		var_to_assign->UnAssign();
		--i; // domain size reduced, go back to original
		if (var_to_assign->IsImpossible())
			return false;
	}
	return false;
}
////////////////////////////////////////////////////////////
//CSP solver, uses arc consistency
template <typename T>
bool CSP<T>::SolveARC(unsigned level) {
	++recursive_call_counter;
	std::vector<Variable*> all_var = cg.GetAllVariables();
	if (cg.AllVariablesAssigned())
	{
		++solution_counter;
		return true;
	}

	//choose a variable by MRV
	Variable* var_to_assign = MinRemVal();

	auto to_save = SaveState(var_to_assign);
	auto domain_x = var_to_assign->GetDomain();
	for(auto var : domain_x)
	{
		++iteration_counter;
		var_to_assign->Assign(var);
		// false, go to next assignment
		// true, go to recursive call
		bool goodToGo = CheckArcConsistency();
		if (goodToGo)
		{
			bool result = SolveARC(level + 1);
			if (result)
				return true;
		}
		LoadState(to_save);
		var_to_assign->RemoveValue(var_to_assign->GetValue());
		var_to_assign->UnAssign();
	}
	return false;
}


template <typename T>
INLINE
bool CSP<T>::ForwardChecking(Variable* x) {
	auto neighbors = cg.GetNeighbors(x);
	for (auto n : neighbors)
	{
		if (n->IsAssigned())
			continue;
		auto connected_constraint = cg.GetConnectingConstraints(x, n);
		for (auto c : connected_constraint)
		{
			if (!RemoveValuesForFC(n, c))
			{
				// Domain wiped out
				if (n->IsImpossible())
					return false;
			}
		}
	}
	return true; // good to go
}
////////////////////////////////////////////////////////////
//load states (available values) of all unassigned variables 
template <typename T>
void CSP<T>::LoadState(
	std::map<Variable*,	std::set<typename CSP<T>::Variable::Value> >& saved) const
{
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator
		b_result = saved.begin();
	typename std::map<Variable*, std::set<typename Variable::Value> >::iterator
		e_result = saved.end();

	for (; b_result != e_result; ++b_result) {
		//std::cout << "loading state for " 
		//<< b_result->first->Name() << std::endl;
		(*b_result).first->SetDomain((*b_result).second);
	}
}


////////////////////////////////////////////////////////////
//save states (available values) of all unassigned variables 
//except the current
template <typename T>
INLINE
std::map< typename CSP<T>::Variable*, std::set<typename CSP<T>::Variable::Value> >
CSP<T>::SaveState(typename CSP<T>::Variable* x) const {
	std::map<Variable*, std::set<typename Variable::Value> > result;

	const std::vector<Variable*>& all_vars = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator
		b_all_vars = all_vars.begin();
	typename std::vector<Variable*>::const_iterator
		e_all_vars = all_vars.end();
	for (; b_all_vars != e_all_vars; ++b_all_vars) {
		if (!(*b_all_vars)->IsAssigned() && *b_all_vars != x) {
			//std::cout << "saving state for " 
			//<< (*b_all_vars)->Name() << std::endl;
			result[*b_all_vars] = (*b_all_vars)->GetDomain();
		}
	}
	return result;
}
////////////////////////////////////////////////////////////
//check the current (incomplete) assignment for satisfiability
template <typename T>
INLINE
bool CSP<T>::AssignmentIsConsistent(Variable* p_var) const {
	// Get all constraints for variable
	const std::vector<const Constraint*>& constraints = cg.GetConstraints(p_var);

	// check satisfiable
	for (const auto& c : constraints) {
		bool s = c->Satisfiable();
		// if one is not, return false
		if (!s) return s;
	}

	// satisfiable
	return true;
}
////////////////////////////////////////////////////////////
//insert pair 
//(neighbors of the current variable, the current variable)
template <typename T>
INLINE
void CSP<T>::InsertAllArcsTo(Variable* cv) {
	auto& neighbors = cg.GetNeighbors(cv);
	for (auto& n : neighbors)
	{
		if (n->IsAssigned())
			continue;
		auto& constraints = cg.GetConnectingConstraints(n, cv);
		for (auto& c : constraints)
		{
			arc_consistency.emplace(n, cv, c);
		}
	}
}
////////////////////////////////////////////////////////////
template <typename T>
INLINE
bool CSP<T>::CheckArcConsistency() {
	auto& all_var = cg.GetAllVariables();
	for (auto& val : all_var)
		InsertAllArcsTo(val);
	while(!arc_consistency.empty())
	{
		Arc<Constraint> curr = (*arc_consistency.begin());
		arc_consistency.erase(curr);
		bool erased = false;
		int x_size = curr.x->SizeDomain();
		if (curr.x->IsAssigned() && !curr.y->IsAssigned())
			RemoveInconsistentValues_one(curr.y, curr.c);
		else if (curr.y->IsAssigned() && !curr.x->IsAssigned())
			RemoveInconsistentValues_one(curr.x, curr.c);
		else
			erased = RemoveInconsistentValues(curr.x, curr.y, curr.c);
		if (curr.x->IsImpossible() || curr.y->IsImpossible())
			return false;
		if (erased)
		{
			// Should add more arc consistency of x and its neighbor
			Variable* changed = curr.y;
			Variable* not_changed = curr.x;
			if (x_size != curr.x->SizeDomain())
			{
				changed = curr.x;
				not_changed = curr.y;
			}
			auto& neighbors = cg.GetNeighbors(changed);
			for (auto& n : neighbors)
			{
				if (n->IsAssigned())
					continue;
				auto& constraints = cg.GetConnectingConstraints(n, changed);
				for (auto& c : constraints)
				{
					if(not_changed->ID() != n->ID())
						arc_consistency.emplace(n, changed, c);
				}
			}
		}
	}
	return true;
}
template <typename T>
INLINE
bool CSP<T>::RemoveValuesForFC(Variable* y, const Constraint* c)
{
	auto domain_y = y->GetDomain();
	for (auto y_val : domain_y)
	{
		y->Assign(y_val);
		// check satisfiable
		if (!(c->Satisfiable()))
			y->RemoveValue(y_val);
		y->UnAssign();
	}
	if (y->SizeDomain() == 0) // Domain Wipe Out
		return false;
	return true;
}
template <typename T>
INLINE
bool CSP<T>::RemoveInconsistentValues_one(Variable* x, const Constraint* c)
{
	bool something_erased = false;
	std::set<Value> domain_x = x->GetDomain();
	for (auto x_val : domain_x)
	{
		x->Assign(x_val);
		if (!c->Satisfiable())
		{
			x->RemoveValue(x_val);
			something_erased = true;
		}
		x->UnAssign();
	}
	return something_erased;
}
////////////////////////////////////////////////////////////
//Check that for each value of x there is a value of y 
//which makes all constraints involving x and y satisfiable
template <typename T>
INLINE
bool CSP<T>::RemoveInconsistentValues(Variable* x, Variable* y, const Constraint* c) {
	std::set<Value> domain_y = y->GetDomain();
	bool something_erased = false;
	for (auto y_val : domain_y)
	{
		y->Assign(y_val);
		auto& domain_x = x->GetDomain();
		bool something_satisfiable = false;
		for (auto x_val : domain_x)
		{
			x->Assign(x_val);
			if (c->Satisfiable())
			{
				something_satisfiable = true;
				x->UnAssign();
				break;
			}
			x->UnAssign();
		}
		if (!something_satisfiable)
		{
			something_erased = true;
			y->RemoveValue(y_val);
		}
		y->UnAssign();
	}
	return something_erased;
}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with minimum remaining values
template <typename T>
INLINE
typename CSP<T>::Variable* CSP<T>::MinRemVal() {
	std::vector<Variable*> rt = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator b_vars = rt.begin();
	typename std::vector<Variable*>::const_iterator e_vars = rt.end();
	Variable* min = nullptr;
	for (; b_vars != e_vars; ++b_vars) {
		if (!(*b_vars)->IsAssigned())
		{
			if (min == nullptr)
				min = (*b_vars);
			else
			{
				if ((*b_vars)->SizeDomain() < min->SizeDomain())
					min = (*b_vars);
			}
		}
	}
	return min;
}
////////////////////////////////////////////////////////////
//choose next variable for assignment
//choose the one with max degree
template <typename T>
typename CSP<T>::Variable* CSP<T>::MaxDegreeHeuristic() {
	std::vector<Variable*> rt = cg.GetAllVariables();
	typename std::vector<Variable*>::const_iterator b_vars = rt.begin();
	typename std::vector<Variable*>::const_iterator e_vars = rt.end();
	Variable* max = rt[0];

	for (; b_vars != e_vars; ++b_vars) {
		unsigned max_size = cg.GetNeighbors(max).size();
		unsigned curr_size = cg.GetNeighbors((*b_vars)).size();
		if (max_size < curr_size)
			max = (*b_vars);
	}
	return max;
}


#undef INLINE_CSP