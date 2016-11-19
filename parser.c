

tSymbolTable *table;
int rule;

int parser(){
	//..
	GetNextToken(FILE *f, int *type, char **content);

	//if( //pokud je soubor prázdný)
	rule = prog();


}

int prog(){
	// zeptat se jestli je vše v pořádku
		if( rule = Class() != 0)
			return 2;
}
	
int Class(){
		if( *type == K_CLASS){
			GetNextToken(FILE *f, int *type, char **content);

			if( strcmp( *(*content), "Main")){
				GetNextToken(FILE *f, int *type, char **content);

				if( *type == B_WATE_LEFT {
					GetNextToken(FILE *f, int *type, char **content);

					if( (rule = Static()) == 0){
						GetNextToken(FILE *f, int *type, char **content);

						if( *type == K_STATIC){
							GetNextToken(FILE *f, int *type, char **content);

							if ( *type == K_VOID){
								GetNextToken(FILE *f, int *type, char **content);

								if( strcmp( *(*content), "run")){
									GetNextToken(FILE *f, int *type, char **content);

									if(*type == B_ROUND_LEFT){
										GetNextToken(FILE *f, int *type, char **content);

										if(*type == B_ROUND_RIGHT){
											GetNextToken(FILE *f, int *type, char **content);

											if( *type == B_WATE_LEFT){
												GetNextToken(FILE *f, int *type, char **content);

												if( (rule = body() == 0)){
													GetNextToken(FILE *f, int *type, char **content);

													if( *type == B_WAWE_RIGHT){
														GetNextToken(FILE *f, int *type, char **content);

														if( rule = Static() == 0){
															GetNextToken(FILE *f, int *type, char **content);

															if( *type == B_WAWE_RIGHT)
																GetNextToken(FILE *f, int *type, char **content);

																if( (rule = Class_next) == 0){
																	return 0;
																}
																else
																	return 2;

															else
																return 2;

														}	
														else
															return 2;

													}
													else
														return 2;

												}
												else 
													return 2;

											}
											else 
												return 2;
										}
										else
											return 2;		
									}
									else
										return 2;

								}
								else
									return 2;

							}
							else 
								return 3;

						}
						else
							return 3;

					}
					else
						return 2;
					
				}
				else
					return 2;

			}
			else
				return 2;
		}	
		else if (*type == K_CLASS){
			GetNextToken(FILE *f, int *type, char **content);

			if( *type == L_SIMPLE){
				GetNextToken(FILE *f, int *type, char **content);

				if( *type == B_WATE_LEFT){
					GetNextToken(FILE *f, int *type, char **content);

					if ( rule = Static() == 0){
						GetNextToken(FILE *f, int *type, char **content);

						if( (rule = body()) == 0){
							GetNextToken(FILE *f, int *type, char **content);

							if( *type == B_WAWE_RIGHT){
								GetNextToken(FILE *f, int *type, char **content);

								if( (rule = Class_next()) == 0){
									return 0;
								}
								else
									return 2;
							}
							else
								return 2;
						}
						else
							return 2;
					}
					else
						return 2;
				}
				else
					return 2;
			}
			else
				return 2;

		}
		else
			return 2;

}

int Class_next(){
	if( *type == K_CLASS){
		return 0;
	}
	if else (..){//epsilon
	}
	else
		return 2;
	
}

int Static(){
	if( (rule = static_id()) == 0){
		return 0;
	if( (rule = static_func()) == 0){
		return 0;
	}
	else
		return 2;
}

int static_id(){
	if( *type == K_STATIC){
		GetNextToken(FILE *f, int *type, char **content);

	}
	else
		return 2;

}

int static_id_expr(){

}

int static_func(){

}

int list_params(){

}

int list_params_next(){

}

int body(){

}

int stat(){

}

int call_func(){

}

int expr(){

}