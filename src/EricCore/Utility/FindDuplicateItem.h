#pragma once

template< typename Return_Type, typename Colls_Iter  > 
struct GetDataPolicyTraits
{
	typedef typename Return_Type  ReturnType;
	typedef typename Colls_Iter   CollsIter;
};

// Policy-based template meta programming  
template<typename RT > 
struct GetDataPolicy;

template< typename RT> 
struct GetDataPolicy < std::vector< RT > >	
{
	typedef GetDataPolicyTraits<RT, typename std::vector<RT>::iterator> Traits;
	typename Traits::ReturnType getItem(typename Traits::CollsIter iter){
		return (*iter);
	}

	string debugString(){
		return "normal type";
	}
};

template< typename RT, typename U > 
struct GetDataPolicy < std::vector< std::pair<RT, U> > >
{
	typedef GetDataPolicyTraits< RT, typename std::vector< std::pair<RT, U> >::iterator > Traits;
	typename Traits::ReturnType getItem(typename Traits::CollsIter iter){
		return (*iter).first;
	}

	string debugString(){
		return "pair type";
	}
};

template< typename T, typename Policy = GetDataPolicy<T> >
class FindDuplicateItem : public Policy
{
public:
	void run(T& source, T& duplicateColl){
		Policy::Traits::CollsIter  iter;
		Policy::Traits::ReturnType addr=0;

		duplicateColl.clear();

		//let sort time approach to Big(1) by using bitMap
		const size_t MAP_SIZE = 0x80000;
		BYTE map[MAP_SIZE]={0};

		for(iter = source.begin(); iter!=source.end(); iter++){
			addr = this->getItem(iter);
			if( _isHit( addr, map, MAP_SIZE) == true ){
				duplicateColl.push_back((*iter));
			}
		}
	}

	string show(){
		return this->debugString();
	}


private:
	bool _isHit(ULONG value, BYTE* pMap, size_t mapSize){

		if( value >=mapSize){
			throw MyException(UTI_PARAM_ERROR, "value>=mapSize"); 
		}

		bool result = true;
		if( pMap[ value ] ==0){
			pMap[ value ] =1;
			result = false;
		}
		return result;
	}
};