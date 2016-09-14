int iskeyword(char const *identifier)
{
  int i;
  for(i = IF;i <= ENDWHILE;i++)
  {
    if(strcmp(keywords[i-IF]) == 0)
      return i;
  }
  return 0;
}