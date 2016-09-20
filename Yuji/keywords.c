

int iskeyword(char const *identifier)
{
  int i;
  for(i = BEGIN;i <= END;i++)
  {
    if(strcmp(keywords[i-IF]) == 0)
      return i;
  }
  return 0;
}