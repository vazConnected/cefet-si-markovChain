# Estimação de Cadeias de Markov com Alcance Variável e suas Aplicações
<details>
  <summary>Resumo</summary>  
    <p>Uma sequencia é modelada por uma Cadeia de Markov quando o valor desta no presente pode ser determinado por parte dos valores do passado.</p>
    <p>Para um modelo convencional de Cadeia de Markov de ordem <em>l</em> e <em>m</em> estados, o número de parâmetros a serem estimados é da ordem de <em>(m-1)*m^t</em>. Se os valores <em>l</em> e <em>m</em> são pequenos, então este modelo é vantajoso, pois há poucos parâmetros a serem estimados e trabalha a questão de dependência do presente no passado. Suponha que <em>l = 2</em> e <em>m = 3</em>, então, teremos 18 parâmetros a serem estimados.</p>
    <p>Uma outra classe de Cadeia de Markov é a de alcance variável, denominada PCT. Assim, não precisamos considerar todos os passados de tamanho <em>k</em> para estimar o próximo símbolo da cadeia. Precisamos apenas de uma subsequência finida do pasado, denominada "<b>Contexto</b>" para estimar o próximo símbolo.</p>
    <p>Aplicações de Cadeias de Markov são encontradas em vários ramos do conhecimento, desde Linguística à Engenharias. A grande vantadem destas aplicações é que o modelo de Markov consegue, considerando um cojunto de dados, estimar o presente considerando uma parte do passado. Isto é importante em muitos processos de produção, em análise de ritmos de textos, estudo de genomas, etc..</p>
    <p>Neste projeto, pretende-se estudar uma aplicação utilizando dados reais.</p>
    <p>Palavras-chave: Cadeia de Markov; Contexto; Estados.
</details>

## Sumário
1. [Gerador interativo de Cadeias de Markov](/1-geradorDeCadeia/geradorDeCadeia.c);
2. [Coletor de informações de uma Cadeia de Markov](/2-coletorDeInformacoes/coletorDeInformacoes.c);
3. [Coletor de informações de uma Cadeia de Markov com Memória Variável](3-coletorDeInformacoesComMemoria/coletorDeInfoemacoesComMemoria.c).


## Créditos
- Autor/Pesquisador: Pedro Vaz ([Lattes](http://lattes.cnpq.br/4701671818864841));
- Professor/Orientador: Silvio Alves ([Lattes](http://lattes.cnpq.br/7731663662793325)).
