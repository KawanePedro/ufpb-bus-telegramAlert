import logging
from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, MessageHandler, filters, ContextTypes
import os
from dotenv import load_dotenv
load_dotenv()
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

# Token do seu bot (substitua pelo seu token real ou use uma variável de ambiente)
BOT_TOKEN = os.getenv('BOT_TOKEN')

# Comando /start
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        f"Olá, {update.effective_user.first_name}! Sou um bot bem burrinho.\n\n"
        "Comandos disponíveis:\n"
        "/help - Mostrar ajuda\n"
        "/info - Informações sobre o bot\n"
        "/echo [mensagem] - Repete sua mensagem\n"
        "/chatid - Mostra o ID deste chat"
    )

# Comando /help
async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Aqui estão os comandos disponíveis:\n"
        "/start - Iniciar o bot\n"
        "/help - Mostrar esta mensagem de ajuda\n"
        "/info - Informações sobre o bot\n"
        "/echo [mensagem] - Repete sua mensagem\n"
        "/chatid - Mostra o ID deste chat"
    )

# Comando /info
async def info(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Avisarei quando o ônibus circular da UFPB estiver próximo do Centro de Informática."
    )

# Comando /echo
async def echo(update: Update, context: ContextTypes.DEFAULT_TYPE):
    # Verifica se o usuário passou algum argumento após o comando
    if context.args:
        message = ' '.join(context.args)
        await update.message.reply_text(f"{message}")
    else:
        await update.message.reply_text("Por favor, escreva alguma mensagem após o comando /echo")

# Novo comando /chatid
async def chatid(update: Update, context: ContextTypes.DEFAULT_TYPE):
    chat_id = update.effective_chat.id
    chat_type = update.effective_chat.type
    chat_title = update.effective_chat.title if update.effective_chat.title else "Chat privado"
    
    await update.message.reply_text(
        f"Informações do chat:\n"
        f"ID: {chat_id}\n"
        f"Tipo: {chat_type}\n"
        f"Nome: {chat_title}\n\n"
        "Use este ID no código da ESP8266 para enviar mensagens para este chat."
    )

# Responder a mensagens normais (não comandos)
async def text_message(update: Update, context: ContextTypes.DEFAULT_TYPE):
    chat_id = update.effective_chat.id
    print(f"Mensagem recebida no chat ID: {chat_id}")
    
    await update.message.reply_text(
        f"Recebi sua mensagem: '{update.message.text}'\n"
        "Use /help para ver os comandos disponíveis."
    )

# Função principal
def main():
    # Criar a aplicação
    application = ApplicationBuilder().token(BOT_TOKEN).build()
    
    # Adicionar handlers de comandos
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("help", help_command))
    application.add_handler(CommandHandler("info", info))
    application.add_handler(CommandHandler("echo", echo))
    application.add_handler(CommandHandler("chatid", chatid))
    
    # Handler para mensagens de texto normais
    application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, text_message))
    
    # Iniciar o bot
    print("Bot iniciado! Pressione Ctrl+C para parar.")
    application.run_polling()


if __name__ == "__main__":
    main()